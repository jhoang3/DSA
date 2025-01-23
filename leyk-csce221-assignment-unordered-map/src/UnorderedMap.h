#include <cstddef>    // size_t
#include <functional> // std::hash
#include <ios>
#include <utility>    // std::pair
#include <iostream>

#include "primes.h"



template <typename Key, typename T, typename Hash = std::hash<Key>, typename Pred = std::equal_to<Key>>
class UnorderedMap {
    public:

    using key_type = Key;
    using mapped_type = T;
    using const_mapped_type = const T;
    using hasher = Hash;
    using key_equal = Pred;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    private:

    struct HashNode {
        HashNode *next;
        value_type val;

        HashNode(HashNode *next = nullptr) : next{next} {}
        HashNode(const value_type & val, HashNode * next = nullptr) : next { next }, val { val } { }
        HashNode(value_type && val, HashNode * next = nullptr) : next { next }, val { std::move(val) } { }
    };

    size_type _bucket_count;
    HashNode **_buckets;

    HashNode * _head;
    size_type _size;

    Hash _hash;
    key_equal _equal;
    
    static size_type _range_hash(size_type hash_code, size_type bucket_count) {
        return hash_code % bucket_count;
    }

    public:

    template <typename pointer_type, typename reference_type, typename _value_type>
    class basic_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = _value_type;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

    private:
        friend class UnorderedMap<Key, T, Hash, key_equal>;
        using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

        const UnorderedMap * _map;
        HashNode * _ptr;

        explicit basic_iterator(UnorderedMap const * map, HashNode *ptr) noexcept { 
            // Creates an iterator to the key-value pair belonging to the HashNode pointed to by ptr.
            _map = map;
            _ptr = ptr;
         }

    public:
        basic_iterator() { 
            _map = nullptr;
            _ptr = nullptr;
        };

        basic_iterator(const basic_iterator &) = default;
        basic_iterator(basic_iterator &&) = default;
        ~basic_iterator() = default;
        basic_iterator &operator=(const basic_iterator &) = default;
        basic_iterator &operator=(basic_iterator &&) = default;
        reference operator*() const { 
            // return a reference to the key-value pair belonging to the _ptr owned by this iterator.
            return _ptr->val;
         }
        pointer operator->() const { return &(_ptr->val); }
        
        // write here
        // check are you iterating in the middle of the linked list?
        // handle the case, jumping from one list to another list
        // search along _buckets until you find a non-empty bucket
        basic_iterator &operator++() { 
            // prefix increment
            /*
            Change the _ptr to be the next _ptr in the UnorderedMap, 
            even if that node is in a different bucket.
            Return a reference to the iterator after the change.
            */
            if(_ptr == nullptr) {
                return *this;
            }
            if(_ptr->next != nullptr) {
                _ptr = _ptr->next;
                return *this;
            }
            // else, find the next non-empty bucket and jump to the head of that bucket
            size_type bucket = _map->_bucket(_ptr->val);
            for(size_type i = bucket +1; i < _map->_bucket_count; i++) {
                if(_map->_buckets[i] != nullptr) {
                    _ptr = _map->_buckets[i];
                    return *this;
                }
            }
            _ptr = nullptr;
            return *this;


         }
        // call prefix increment
        basic_iterator operator++(int) { 
            // postfix increment
            // dont increment if next node is nullptr
            if(_ptr == nullptr ) {
                return *this;
            }
            basic_iterator copy = *this;
            ++(*this);
            return copy;
        }
        bool operator==(const basic_iterator &other) const noexcept { return _ptr == other._ptr; }
        bool operator!=(const basic_iterator &other) const noexcept { return _ptr != other._ptr; }
    };

    using iterator = basic_iterator<pointer, reference, value_type>;
    using const_iterator = basic_iterator<const_pointer, const_reference, const value_type>;

    class local_iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = std::pair<const key_type, mapped_type>;
            using difference_type = ptrdiff_t;
            using pointer = value_type *;
            using reference = value_type &;

        private:
            friend class UnorderedMap<Key, T, Hash, key_equal>;
            using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

            HashNode * _node;

            /*
            Creates a local_iterator to the key-value pair belonging to the HashNode 
            pointed to by ptr limited to the bucket bucket within map.
            */
            explicit local_iterator( HashNode * node ) noexcept { 
                _node = node;
             }

        public:
            // basically like linked list iterator
            // Creates a local_iterator by default, pointer belonging to the local iterator = nullptr.

            local_iterator() { 
                _node = nullptr;
             }

            local_iterator(const local_iterator &) = default;
            local_iterator(local_iterator &&) = default;
            ~local_iterator() = default;
            local_iterator &operator=(const local_iterator &) = default;
            local_iterator &operator=(local_iterator &&) = default;
            // Return a reference to the key-value pair belonging to the _node owned by this iterator.
            reference operator*() const { return _node->val; }
            pointer operator->() const { return &(_node->val); }
            local_iterator & operator++() {  // prefix increment
                // dont increment if next node is nullptr
                if(_node == nullptr ) {
                    return *this;
                }
                _node = _node->next;
                return *this;
             }
            local_iterator operator++(int) { // postfix increment
                // dont increment if next node is nullptr
                if(_node == nullptr ) {
                    return *this;
                }
                local_iterator copy = *this;
                _node = _node->next;
                return copy;
             }

            bool operator==(const local_iterator &other) const noexcept { return _node == other._node; }
            bool operator!=(const local_iterator &other) const noexcept { return _node != other._node; }
    };

private:
    
    // returns the bucket index for the given hash code
    size_type _bucket(size_t code) const { return _range_hash(code, _bucket_count); }
    // returns the bucket index for the given key: hash the key and then find the bucket index
    size_type _bucket(const Key & key) const { return _bucket(_hash(key)); }
    // returns the bucket index for the given value: 
    // val.first is the key since value_type's first value is the key
    size_type _bucket(const value_type & val) const { return _bucket(val.first); }

    /*Starts with the nodes in bucket bucket and iterates forward until the key matches key, 
    returning the node where the keys match. 
    If no such match occurs, returns nullptr.*/
    HashNode*& _find(size_type code, size_type bucket, const Key & key) { 
        // must return a pointer reference
        HashNode** node = &_buckets[bucket]; // get the head of the bucket
        while(*node) {
            if(_equal((*node)->val.first, key)) {
                return *node;
            }
            node = &((*node)->next); // iterate
        }
        return *node;
     }

    // call above with the given key
    HashNode*& _find(const Key & key) { return _find(_hash(key), _bucket(key), key); }
    
    // insert a pair as the new bucket's head, use move semantics
    HashNode * _insert_into_bucket(size_type bucket, value_type && value) {
        // moves the value, sets the next node to the current head of the bucket 
        HashNode* node = new HashNode(std::move(value), _buckets[bucket]); 
        _buckets[bucket] = node; // set new node as new head of the bucket

        // if global head is empty or the bucket index of _head > value's bucket index, update _head
        if(_head == nullptr || _bucket(_head->val) >= bucket) {
            _head = node;
        }
        _size++; // increment size
        return node;   
     }

    void _move_content(UnorderedMap & src, UnorderedMap & dst) { 
        // move content from src to dst without new allocations
        // copy the content of other to this
     }

public:

    // construct _buckets array
    // size _buckets array should be a prime number
    // initialize array with nullptr
    // Ptr* new_node = new Ptr [size]{};

    explicit UnorderedMap(size_type bucket_count, const Hash & hash = Hash { },
                const key_equal & equal = key_equal { }) { 
                    // default constructor
                    _bucket_count = next_greater_prime(bucket_count);
                    _buckets = new HashNode *[_bucket_count] { nullptr };
                    _size = 0;
                    _hash = hash;
                    _equal = equal;
                    _head = nullptr;
                }
    // destructor
    ~UnorderedMap() { 
        clear();
        delete[] _buckets;
     }

    // copy constructor
    UnorderedMap(const UnorderedMap & other) { 
        // copy the content of other to this
        _bucket_count = other._bucket_count;
        _buckets = new HashNode *[_bucket_count] { nullptr };
        _size = 0;
        _hash = other._hash;
        _equal = other._equal;
        _head = nullptr;
        // copy nodes
        for(auto it = other.cbegin(); it != other.cend(); ++it) {
            insert(*it);
        }
     }

    // move constructor
    UnorderedMap(UnorderedMap && other) { 
        // move the content of other to this
        /*
        Constructs the container with the contents of other using move semantics. 
        Ensure other is left in a useable state, with empty buckets.
        */
        
        // dont zero out bucket count since its like capacity
        _buckets = other._buckets;
        other._buckets = new HashNode* [other._bucket_count]();

        _hash = other._hash;
        _equal = other._equal;
        _head = other._head;
        _size = other._size;
        _bucket_count = other._bucket_count;

        // zero out everything in other
        other._head = nullptr;
        other._size = 0;
        other._hash = Hash();
        other._equal = key_equal();

     }

    // copy assignment
    UnorderedMap & operator=(const UnorderedMap & other) { 
        // copy the content of other to this
        if(this != &other) {
            clear();
            delete[] _buckets;
            _bucket_count = other._bucket_count;
            _buckets = new HashNode *[_bucket_count] { nullptr };
            _size = 0;
            _hash = other._hash;
            _equal = other._equal;
            _head = nullptr;
            // copy nodes
            for(auto it = other.cbegin(); it != other.cend(); ++it) {
                insert(*it);
            }
        }
        return *this;
     }

    // move assignment
    UnorderedMap & operator=(UnorderedMap && other) { 
        if(this != &other) {
            clear();
            delete[] _buckets;
            _buckets = other._buckets;
            other._buckets = new HashNode* [other._bucket_count]();

            _hash = other._hash;
            _equal = other._equal;
            _size = other._size;
            _bucket_count = other._bucket_count;

            // zero out everything in other
            other._head = nullptr;
            other._size = 0;
            other._hash = Hash();
            other._equal = key_equal();
        }
        return *this;
     }

    // call erase on _head 
    void clear() noexcept { 
        // clear all the nodes in the map
        while(_head) { // call erase while head is not nullptr
            erase(iterator(this, _head));
        }

     }

    size_type size() const noexcept { return _size; }

    bool empty() const noexcept { return _size == 0; }

    size_type bucket_count() const noexcept { 
        // returns number of buckets
        return _bucket_count;
    }
    // returns an iterator to the first element of the map
    iterator begin() { return iterator(this, _head); }
    // returns an iterator to the element following the last element of the map
    iterator end() { return iterator(this, nullptr); }

    const_iterator cbegin() const { 
        // returns a const iterator to the first element of the map
        return const_iterator(this, _head);
     };
    const_iterator cend() const { 
        return const_iterator(this, nullptr);
     };

    local_iterator begin(size_type n) { 
        // Returns a local iterator to the first element of the bucket with index n.
        return local_iterator(_buckets[n]);
     }
    local_iterator end(size_type n) { 
        // Returns a local iterator to the element following the last element of the bucket with index n.
        return local_iterator(nullptr);
    }

    size_type bucket_size(size_type n) { 
        // returns the number of elements in the bucket
        size_type count = 0;
        HashNode * node = _buckets[n]; // first node in the bucket
        while(node) {
            count++;
            node = node->next; // iterate and count
        }
        return count;
    }
    // static cast to float, returns average # of elements per bucket
    float load_factor() const { return  static_cast<float>( size() )/ bucket_count(); }

    size_type bucket(const Key & key) const { 
        /*
         Returns the index of the bucket for key. Elements (if any) with keys 
         equivalent to key are always found in this bucket. The returned value is valid only 
         for instances of the container for which bucket_count() returns the same value.
        */
        return _bucket(key);
    }


    /*
    Inserts value using move semantics. Returns a pair consisting of an iterator
     to the inserted element (or to the element that prevented the insertion) 
    and a bool denoting whether the insertion took place 
    (true if insertion happened, false if it did not).
    */
    std::pair<iterator, bool> insert(value_type && value) { 
        size_type bucket = _bucket(value); // get the bucket index
        // insertion fails when the key already exists
        // search for the key in the bucket
        HashNode* node = _find(_hash(value.first), bucket, value.first);
        if(node == nullptr) {
            return std::make_pair(iterator(this, _insert_into_bucket(bucket, std::move(value))), true);
        }
        return std::make_pair(iterator(this, node), false);
     }

    std::pair<iterator, bool> insert(const value_type & value) { 
        // using copy semantics
        // how do you use move semantics here?
        // hint: construct an rvalue reference from value
        // _insert_into_bucket with T(value)
        return insert(value_type(value));
     }

    iterator find(const Key & key) { 
        /*
         Finds an element with key equivalent to key. 
         If no such element is found, past-the-end (see end()) iterator is returned.
        */
        HashNode* node = _find(key);
        if(node == nullptr) {
            return end();
        }
        return iterator(this, node);
     }

    // read doc. try to find key, if you can't find it, insert a "fake key"
    T& operator[](const Key & key) { 
        /*
         Inserts a value_type object constructed in-place if the key does not exist. 
         Returns a reference to the mapped value of the new element 
         if no element with key key existed. 
         Otherwise, returns a reference to the mapped value of the existing 
         element whose key is equivalent to key.
        */
        iterator it = find(key);
        if(it == end()) {
            return insert(std::make_pair(key, T())).first->second;
        }
        return it->second;
     }

    // find node
    // return an iterator following the removed element
    // think about _head
    // return end() if the element is not found
    // think about pointer reference
    iterator erase(iterator pos) { 
        /*
        Removes the element at pos. The given iterator pos must be valid and able to be dereferenced 
        (you don't have to check this and can assume it to be true). 
        Thus the end() iterator (which is valid, but is not able to be dereferenced) 
        cannot be used as a value for pos. Returns an iterator following the last removed element.
        */
        // _find returns a pointer reference, use that to change what the pointer points to
        // if the node is the head, update the head
        HashNode*& ptr = _find(pos->first);     
        ++pos;
        // get node that ptr points to
        HashNode* node = ptr;
        // if node is the head, update head
        if(node == _head) {
            // set head to the next node in the map
            _head = pos._ptr;
        }
        ptr = ptr->next; // update the pointer to point to the next node
        delete node; // delete the node
        _size--; // decrement size
        return pos;
     }

    // return 0 or 1 depending on completion
    size_type erase(const Key & key) { 
        // use the previous function
        // if the iterator is not end(), return 1
        // else return 0
        iterator it = find(key);
        if(it != end()) {
            erase(it);
            return 1;
        }
        return 0;
     }

    template<typename KK, typename VV>
    friend void print_map(const UnorderedMap<KK, VV> & map, std::ostream & os);
};

template<typename K, typename V>
void print_map(const UnorderedMap<K, V> & map, std::ostream & os = std::cout) {
    using size_type = typename UnorderedMap<K, V>::size_type;
    using HashNode = typename UnorderedMap<K, V>::HashNode;

    for(size_type bucket = 0; bucket < map.bucket_count(); bucket++) {
        os << bucket << ": ";

        HashNode const * node = map._buckets[bucket];

        while(node) {
            os << "(" << node->val.first << ", " << node->val.second << ") ";
            node = node->next;
        }

        os << std::endl;
    }
}
