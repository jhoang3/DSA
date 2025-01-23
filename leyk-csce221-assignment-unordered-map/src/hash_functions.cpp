#include "hash_functions.h"

size_t polynomial_rolling_hash::operator() (std::string const & str) const {
    /* TODO */
    /*
    Returns the hash code resulting from hashing str using a polynomial rolling hash algorithm. 
    To pass our test cases, you will need to have b be 19 and m be 3298534883309ul.
    */
    size_t b = 19;
    size_t m = 3298534883309ul;
    size_t p = 1;
    size_t hash = 0;
    for(size_t i = 0; i < str.size(); i++) {
        hash += str[i] * p;
        p = (p * b) % m;
    }
    return hash;
}

size_t fnv1a_hash::operator() (std::string const & str) const {
    /* TODO */
    /*
    Returns the hash code resulting from hashing str using the fnv1a algorithm. To pass our test cases, 
    you will need to have the prime be 0x00000100000001B3 and the basis be 0xCBF29CE484222325.*/
    size_t prime = 0x00000100000001B3;
    size_t basis = 0xCBF29CE484222325;
    size_t hash = basis;
    for(size_t i = 0; i < str.size(); i++) {
        hash ^= str[i];
        hash *= prime;
    }
    return hash;
}
