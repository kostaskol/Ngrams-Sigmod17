#include <stdexcept>
#include "array.h"

template <typename T>
mstd::array<T>::array(size_t size)
        : _size(size), _used(0) {
    _arr = new T[_size];
}

template <typename T>
mstd::array<T>::array(const array &other)
        : _size(other._size), _used(other._used) {
    _arr = new T[_size];
    for (int i = 0; i < _size; i++) {
        _arr[i] = other._arr[i];
    }
}

template <typename T>
mstd::array<T>::~array() {
    delete[] _arr;
}

template <typename T>
void mstd::array<T>::set(int index, T value) {
    if (index >= _size) {
        throw std::runtime_error("Index out of range");
    }


    _arr[index] = value;
    _used++;
}

template <typename T>
T &mstd::array<T>::get(int index) {

}

template <typename T>
void mstd::array<T>::resize() {

}

template <typename T>
void mstd::array<T>::shrink() {

}

template <typename T>
bool mstd::array<T>::full() {

}

template <typename T>
size_t mstd::array<T>::size() {

}

template <typename T>
size_t mstd::array<T>::used() {

}

template <typename T>
T &mstd::array<T>::operator[](int index) {

}

template <typename T>
mstd::array<T> &mstd::array<T>::operator=(const mstd::array<T> &other) {

}