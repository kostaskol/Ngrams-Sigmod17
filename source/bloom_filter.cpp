#include "helpers.hpp"
#include "bloom_filter.hpp"
#include <string>
#include <iostream>
#include <cstring>

#define BIG_CONSTANT(x) (x##LLU)

uint64_t rotl64(uint64_t x, uint64_t r) {
    return (x << r) | (x >> (64 - r));
}

using mstd::vector;
using std::string;
using std::cout;
using std::endl;

bloom_filter::bloom_filter(size_t size, int k) : _bv(size), _size(size), _k(k) {
    _results = new size_t[k];
}

bloom_filter::~bloom_filter() {
    delete[] _results;
}

bool bloom_filter::check(const string &ngram) {
    _hash(ngram);
    for (int i = 0; i < _k; i++) {
        if (!_bv.check(_results[i])) {
            return false;
        }
    }

    return true;
}

void bloom_filter::insert(const string &ngram) {
    _hash(ngram);
    for (int i = 0; i < _k; i++) {
        _bv.set(_results[i]);
    }
}

// Sets all of the hash's result indices to 1
// Returns true if all of them were already set, false otherwise
bool bloom_filter::check_and_set(const string &ngram) {
    _hash(ngram);
//    int matched = 0;
    bool exists = true;
    for (int i = 0; i < _k; i++) {
        if (!_bv.check_and_set(_results[i])) {
            // Count the number of times a bit was set
            exists = false;
        }
    }

    // If all of the bits were set, we return a positive (false or proper)
    return exists;
}

uint64_t fmix(uint64_t h) {
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

// Hashes the given string and
// populates an array of indices (_results)
// Uses the murmur 3 hash
void bloom_filter::_hash(const string &str) {
    auto *s = new char[str.length() + 1];
    strcpy(s, str.c_str());
    void *c = s;
    auto *data = (const uint8_t*) c;
    const int nblocks = str.length() / 16;

    uint64_t h1 = 0;
    uint64_t h2 = 0;

    const uint64_t c1 = BIG_CONSTANT(0x87c37b91114253d5);
    const uint64_t c2 = BIG_CONSTANT(0x4cf5ad432745937f);

    auto *blocks = (const uint64_t *)data;

    for (int i = 0; i < nblocks; i++) {
        uint64_t k1 = blocks[i * 2];
        uint64_t k2 = blocks[i * 2 + 1];

        k1 *= c1; k1 = rotl64(k1, 31); k1 *= c2; h1 ^= k1;

        h1 = rotl64(h1, 27); h1 += h2; h1 = h1 * 5 + 0x52dce729;

        k2 *= c2; k2 = rotl64(k2, 33); k2 *= c1; h2 ^= k2;

        h2 = rotl64(h2, 31); h2 += h1; h2 = h2 * 5 + 0x38495ab5;
    }

    auto *tail = (data + nblocks * 16);

    uint64_t k1 = 0;
    uint64_t k2 = 0;

    switch (str.length() & 15) {
        case 15: k2 ^= ((uint64_t)tail[14]) << 48;
        case 14: k2 ^= ((uint64_t)tail[13]) << 40;
        case 13: k2 ^= ((uint64_t)tail[12]) << 32;
        case 12: k2 ^= ((uint64_t)tail[11]) << 24;
        case 11: k2 ^= ((uint64_t)tail[10]) << 16;
        case 10: k2 ^= ((uint64_t)tail[ 9]) << 8;
        case  9: k2 ^= ((uint64_t)tail[ 8]) << 0;
            k2 *= c2; k2  = rotl64(k2,33); k2 *= c1; h2 ^= k2;

        case  8: k1 ^= ((uint64_t)tail[ 7]) << 56;
        case  7: k1 ^= ((uint64_t)tail[ 6]) << 48;
        case  6: k1 ^= ((uint64_t)tail[ 5]) << 40;
        case  5: k1 ^= ((uint64_t)tail[ 4]) << 32;
        case  4: k1 ^= ((uint64_t)tail[ 3]) << 24;
        case  3: k1 ^= ((uint64_t)tail[ 2]) << 16;
        case  2: k1 ^= ((uint64_t)tail[ 1]) << 8;
        case  1: k1 ^= ((uint64_t)tail[ 0]) << 0;
            k1 *= c1; k1 = rotl64(k1,31); k1 *= c2; h1 ^= k1;
    }

    h1 ^= str.length(); h2 ^= str.length();
    h1 += h2;
    h2 += h1;

    h1 = fmix(h1);
    h2 = fmix(h2);

    h1 += h2;
    h2 += h1;

    h1 = h1 % _size;
    h2 = h2 % _size;

    for (int i = 0; i < _k; i++) {
        _results[i] = (h1 + i * h2 + i*i) % _size;
    }

    delete[] s;
}
