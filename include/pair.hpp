#ifndef PAIR_HPP
#define PAIR_HPP
#include <string>
#include <iostream>

class pair {
private:
    std::string _word;
    size_t _freq;
public:
    pair() : _word(""), _freq(0) {}

    pair(const std::string &word, size_t freq = 1) : _word(word), _freq(freq) {}

    pair(const pair &other) noexcept {
        _word = other._word;
        _freq = other._freq;
    }

//    pair(pair &&other) noexcept {
//        _word = other._word;
//        _freq = other._freq;
//    }

    ~pair()=default;

    std::string &get_word() { return _word; }

    size_t get_freq() { return _freq; }

    void count() { _freq++; }

    void print() {
        std::cout << _word << " : " << _freq;
    }

    bool operator<(const pair &a) noexcept{
        return this->_word < a._word;
    }

    pair &operator=(const pair &other) noexcept {
        _word = other._word;
        _freq = other._freq;
        return *this;
    }

//    pair &operator=(pair &&other) noexcept {
//        _word = other._word;
//        _freq = other._freq;
//        return *this;
//    }

};

template <typename T, typename V>
class tuple {
public:
    T a;
    V b;

    tuple() { }
    tuple(T a, V b) : a(a), b(b) { }
};

#endif //PAIR_HPP
