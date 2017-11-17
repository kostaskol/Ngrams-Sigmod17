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

bloom_filter::bloom_filter(size_t size, int k) : _size(size), _bv(size), _k(k) {
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

bool bloom_filter::check(const vector<string> &ngram_v) {
    string ngram = helpers::join(ngram_v, ' ');
    return check(ngram);
}

void bloom_filter::insert(const string &ngram) {
    _hash(ngram);
    for (int i = 0; i < _k; i++) {
        _bv.set(_results[i]);
    }
}

void bloom_filter::insert(const vector<string> &ngram_v) {
    string ngram = helpers::join(ngram_v, ' ');
    insert(ngram);
}

bool bloom_filter::check_and_set(const string &ngram) {
    _hash(ngram);
    bool exists = false;
    for (int i = 0; i < _k; i++) {
        exists = exists || _bv.check_and_set(_results[i]);
    }

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

bool bloom_filter::check_and_set(const vector<string> &ngram_v) {
    string ngram = helpers::join(ngram_v, ' ');
    return check_and_set(ngram);
}


void bloom_filter::_hash(const string &str, int hash) {
    auto *s = new char[str.length() + 1];
    strcpy(s, str.c_str());
    // Calculate h1
    // Jenkins hash
    if (hash == JENKINS_PEARSONS) {
        int hash = 0;
        char *c = s;
        while (*c != '\0') {
            hash += *c++;
            hash += (hash << 10);
            hash ^= (hash >> 6);
        }

        hash += (hash << 3);
        hash ^= (hash >> 11);
        hash += (hash << 15);

        hash = hash >= 0 ? hash : -hash;
        _results[0] = hash % _size;

        // Pearson hash
        static const unsigned char T[256] = {
                98,  6, 85, 150, 36, 23, 112, 164, 135, 207, 169, 5, 26, 64, 165, 219, //  1
                61, 20, 68, 89, 130, 63, 52, 102, 24, 229, 132, 245, 80, 216, 195, 115, //  2
                90, 168, 156, 203, 177, 120, 2, 190, 188, 7, 100, 185, 174, 243, 162, 10, //  3
                237, 18, 253, 225, 8, 208, 172, 244, 255, 126, 101, 79, 145, 235, 228, 121, //  4
                123, 251, 67, 250, 161, 0, 107, 97, 241, 111, 181, 82, 249, 33, 69, 55, //  5
                59, 153, 29, 9, 213, 167, 84, 93, 30, 46, 94, 75, 151, 114, 73, 222, //  6
                197, 96, 210, 45, 16, 227, 248, 202, 51, 152, 252, 125, 81, 206, 215, 186, //  7
                39, 158, 178, 187, 131, 136, 1, 49, 50, 17, 141, 91, 47, 129, 60, 99, //  8
                154, 35, 86, 171, 105, 34, 38, 200, 147, 58, 77, 118, 173, 246, 76, 254, //  9
                133, 232, 196, 144, 198, 124, 53, 4,108, 74, 223, 234, 134, 230, 157, 139, // 10
                189, 205, 199, 128, 176, 19, 211, 236, 127, 192, 231, 70, 233, 88, 146, 44, // 11
                183, 201, 22, 83, 13, 214, 116, 109, 159, 32, 95, 226, 140, 220, 57, 12, // 12
                221, 31, 209, 182, 143, 92, 149, 184, 148, 62, 113, 65, 37, 27, 106, 166, // 13
                3, 14, 204, 72, 21, 41, 56, 66, 28, 193, 40, 217, 25, 54, 179, 117, // 14
                238, 87, 240, 155, 180, 170, 242, 212, 191, 163, 78, 218, 137, 194, 175, 110, // 15
                43, 119, 224, 71, 122, 142, 42, 160, 104, 48, 247, 103, 15, 11, 138, 239  // 16
        };

        hash = 0;
        c = s;
        while (*c != '\0') {
            int index = hash ^ *c++;
            hash = T[index];
        }

        hash = hash >= 0 ? hash : -hash;

        _results[1] = hash % _size;
    } else {
    // Murmur3
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

        _results[0] = h1 % _size;
        _results[1] = h2 % _size;
    }



    for (int i = 2; i < _k; i++) {
        _results[i] = (_results[0] * i * _results[1]) % _size;
    }
}
