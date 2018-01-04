#include "bit_vector.hpp"
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <sstream>

using std::cout;
using std::endl;

bit_vector::bit_vector(size_t size) {
    // Since we're allocating bytes, the number of bits should be a multiple of 8
    if (size % 8 != 0) throw std::runtime_error("size should be a multiple of 8");
    size_t actual = size >> 3;
    //                          v Important! Initialises array to 0
    _entries = new byte[actual]();
    _size = size;
}

bit_vector::bit_vector(const bit_vector &other) {
    size_t actual = other._size >> 3;
    _entries = new byte[actual];
    for (size_t i = 0;i < actual; i++) {
        _entries[i] = other._entries[i];
    }
    _size = other._size;
}

bit_vector::~bit_vector() {
    delete[] _entries;
}

bool bit_vector::_set(byte &b, byte offset) {
    b |= (0x1 << (int) offset);
    return false;
}

bool bit_vector::_test(byte b, byte offset) {
    return (bool) (b & (0x1 << (int) offset));
}

void bit_vector::set(size_t index) {
    if (index >= _size) {
        std::stringstream ss("The requested index (");
        ss << index << ") was out of range";
        throw std::out_of_range(ss.str());
    }
    size_t byte_index = index / 8;
    auto offset = (byte) (7 - (index % 8));
    _set(_entries[byte_index], offset);
}

bool bit_vector::check(size_t index) {
    if (index >= _size) {
        throw std::out_of_range("The requested index (" + std::to_string(index) + ") was out of range");
    }
    size_t byte_index = index / 8;
    auto inner_index = (byte) (index % 8);
    byte b = _entries[byte_index];
    auto offset = (byte) (7 - inner_index);
    return _test(b, offset);
}

/*
 * Returns true if bit was set
 * Returns false if bit was not set (and sets it)
 */
bool bit_vector::check_and_set(size_t index) {
    if (index >= _size) {
        throw std::out_of_range("The requested index (" + std::to_string(index) + ") was out of range");
    }

    size_t byte_index = index / 8;
    auto inner_index = (byte) (index % 8);
    auto offset = (byte) (7 - inner_index);
    byte b = _entries[byte_index];

    if (_test(b, offset)) {
        return true;
    }

    _set(_entries[byte_index], offset);
    return false;
}

bit_vector &bit_vector::operator=(const bit_vector &other) {
    delete[] _entries;
    size_t actual = other._size >> 3;
    _entries = new byte[actual];
    for (size_t i = 0; i < actual; i++) {
        _entries[i] = other._entries[i];
    }
    _size = other._size;
    return *this;
}
