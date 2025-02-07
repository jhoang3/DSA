#include "executable.h"

TEST(constructor_move) {
    Typegen t;
    for(size_t i = 0; i < TEST_ITER; i++) {

        using Map = UnorderedMap<double, double>;
        using value_type = std::pair<double, double>;
        
        size_t n_pairs = t.range(1000ul);

        std::vector<value_type> pairs(n_pairs);
        t.fill(pairs.begin(), pairs.end());

        size_t n = t.range(100ull);
        Map map(n);
        shadow_map<double, double> shad_map(n);

        for(auto const & pair : pairs) {
            map.insert(pair);
            shad_map.insert(pair);
        }

        ASSERT_PAIRS_FOUND_IN_CORRECT_BUCKETS(shad_map, map);

        {
            Memhook mh;

            Map cpy_map { std::move(map) };

            ASSERT_EQ(0ULL, mh.n_frees()); // no deallocations
            ASSERT_EQ(1ULL, mh.n_allocs()); // 1 allocation
            ASSERT_EQ(shad_map.size(), cpy_map.size());

            ASSERT_PAIRS_FOUND_IN_CORRECT_BUCKETS(shad_map, cpy_map);

            size_t n_new_cpy_pairs = t.range(1000ul);
            size_t n_new_pairs = t.range(1000ul);

            std::vector<value_type> new_cpy_pairs(n_new_cpy_pairs);
            t.fill(new_cpy_pairs.begin(), new_cpy_pairs.end());
            
            std::vector<value_type> new_pairs(n_new_pairs);
            t.fill(new_pairs.begin(), new_pairs.end());

            for(auto const & pair : new_cpy_pairs) {
                shad_map.insert(pair);
                cpy_map.insert(pair);
            }

            ASSERT_PAIRS_FOUND_IN_CORRECT_BUCKETS(shad_map, cpy_map);
            
            shadow_map<double, double> moved_shad_map(n);

            for(auto const & pair : new_pairs) {
                map.insert(pair);
                moved_shad_map.insert(pair);
            }

            ASSERT_PAIRS_FOUND_IN_CORRECT_BUCKETS(moved_shad_map, map);
        }

    }
}