#ifndef TRIES_BLOOM_FILTER_HPP
#define TRIES_BLOOM_FILTER_HPP

#include "bit_vector.hpp"
#include <string>
#include "mvector.hpp"
#include "constants.hpp"

class bloom_filter {
private:
    bit_vector _bv;
    size_t _size;
    size_t *_results;
    int _k;

    void _hash(const std::string &s);
public:
    bloom_filter(size_t size, int k);
    bloom_filter(const bloom_filter &)=delete;
    ~bloom_filter();

    bool check(const std::string &ngram);

    void insert(const std::string &ngram);

    bool check_and_set(const std::string &ngram);

    bloom_filter &operator=(const bloom_filter &)=delete;
};

#endif //TRIES_BLOOM_FILTER_HPPPP