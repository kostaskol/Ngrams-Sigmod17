#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <iostream>
#include <cstdlib>

// Simple resizable array template class
// that includes some of std::vector's basic operations


// NOTE: mstd::vector<T*> takes ownership of the passed pointers instead of copying them
// That means that if the pointers are deleted elsewhere, the program will *hopefully* crash
namespace mstd {
    template <typename T>
    class vector {
    private:
        size_t _size{};
        size_t _capacity;
        int _last;
        T *_entries;

        void _enlarge() {
            auto *tmp = new T[_capacity << 1];
            std::copy(_entries, _entries + _size, tmp);

            delete[] _entries;
            _entries = tmp;

            _capacity = _capacity << 1;
        }

    public:
        explicit vector(size_t capacity = 1)
                : _size(0), _capacity(capacity), _last(0) {
            _entries = new T[_capacity];
        }

        vector(const vector &other)
                : _size(other._size),
                  _capacity(other._capacity) {
            _entries = new T[_capacity];
            std::copy(other._entries, other._entries + other._size, _entries);
            _last = other._last;
        }

        vector(const vector &other, int start, int end) {
            if (start < 0 || end > (int) other._size) return;
            _capacity = other._capacity;
            _size = (size_t) end - start;
            _last = other._last;
            _entries = new T[_capacity];
            int cur = 0;
            std::copy(other._entries + start, other._entries + end, _entries);
        }

        vector(vector &&other) noexcept : vector() {
            // other object is swapped with this (which has been instantiated and is thus safe to delete)
            _swap_vectors(*this, other);
        }



        ~vector() {
            delete[] _entries;
        }

        vector sublist(int start, int length) const {
            if (start + length > (int) _size) {
                throw std::runtime_error("Bad indices");
            }

            vector tmp;
            for (auto i = (size_t) start; i <= (size_t) start + length; i++) {
                tmp.push(_entries[i]);
            }

            return tmp;
        }

        void push(const T &ent) {
            if (_size + 1>= (size_t) _capacity) {
                _enlarge();
            }

            _entries[_last++] = T(ent);
            _size++;
        }

        void m_push(T &ent) {
            if (_size + 1 >= (size_t) _capacity) _enlarge();

            _entries[_last++] = std::move(ent);
            _size++;
        }

        T *get_last_inserted() const {
            return &_entries[_last - 1];
        }

        void add(const T &ent) {
            push(ent);
        }

        void insert_at(int index, const T &ent) {
            if (index > _size) {
                throw std::runtime_error("Index out of range");
            }

            if (_size + 1>= (size_t) _capacity) {
                _enlarge();
            }

            if ((_size == 0) || (index == _size)) {
                push(ent);
                return;
            }

            for (size_t i = _size - 1; i >= index; i--) {
                _entries[i + 1] = std::move(_entries[i]);
            }

            _entries[index] = T(ent);
            _size++;
        }

        void m_insert_at(int index, T &ent) {
            if (_size + 1 >= (size_t) _capacity) _enlarge();

            if ((_size == 0) || (index == _size)) {
                m_push(ent);
                return;
            }

            for (size_t i = _size - 1; i >= index; i--) {
                _entries[i + 1] = std::move(_entries[i]);
            }

            _entries[index] = std::move(ent);
            _size++;
        }

        void shrink_to_size() {
            auto *tmp = new T[_size];
            for (int i = 0; i < _size; i++) {
                tmp[i] = _entries[i];
            }
            delete[] _entries;
            _entries = tmp;
            _capacity = _size;
        }

        bool in(const T &ent) const {
            for (size_t i = 0; i < _size; i++) {
                if (_entries[i] == ent) {
                    return true;
                }
            }
            return false;
        }

        T &at(int index) const {
            if (index >= (int) _size) {
                throw std::runtime_error("Index out of range:");
            }

            return _entries[index];
        }

        // Method sugar
        T &get(int index) const {
            return at(index);
        }



        T *at_p(int index) {
            if (index >= (int) _size) {
                throw std::runtime_error("Index out of range");
            }

            return &_entries[index];
        }

        // Method sugar
        T *get_p(int index) {
            return at_p(index);
        }

        int get_index(const T &ent) const {
            for (size_t i = 0; i < _size; i++) {
                if (_entries[i] == ent) return (int) i;
            }
            return -1;
        }

        void clear(size_t new_cap = 1) {
            delete[] _entries;
            _size = 0;

            _last = 0;

            _capacity = new_cap;
            _entries = new T[_capacity];
        }

        void set_at(int index, const T &ent) {
            if (index < 0 || index > (int) _size) {
                throw std::runtime_error("Bad index: " + index);
            }

            _entries[index] = T(ent);
        }


        bool remove(const T &ent) {
            auto *tmp = new T[_capacity];
            int j = 0;
            bool found = false;
            size_t i;
            for (i = 0; i < _size; i++) {
                if (_entries[i] == ent) break;
            }

            for (size_t j = _size - 1; j >= i; j++) {
            }
            for (size_t i = 0; i < _size; i++) {
                if (_entries[i] == ent) {
                    found = true;
                    continue;
                }

                tmp[j++] = _entries[i];
            }

            if (found) {
                delete[] _entries;
                _entries = tmp;

                _size--;
                _last--;
            } else {
                delete[] tmp;
            }

            return found;
        }

        // Iterators. Allow for foreach loops
        // (for (int i : vec))
        T *begin() {
            return &_entries[0];
        }

        T *end() {
            return &_entries[_last];
        }

        void remove_at(int index) {
            if (index >= (int) _size || index < 0) {
                throw std::runtime_error("Bad index: " + index);
            }
            auto *tmp_ent = new T[_capacity];

            int j = 0;
            for (size_t i = 0; i < _size; i++) {
                if ((int) i != index) {
                    tmp_ent[j++] = _entries[i];
                }
            }

            delete[] _entries;

            _entries = tmp_ent;

            _size--;
            _last--;
        }

        size_t size() const { return _size; }



        T get_cpy(int index) const {
            if (index >= _size) {
                throw std::runtime_error("Index out of range");
            }

            return _entries[index];
        }

        T &operator[](int index) const {
            return at(index);
        }

        // Copy operator=
        vector &operator=(const vector &other) {
            vector tmp(other);
            _swap_vectors(*this, tmp);

            return *this;
        }

        // Move operator=
        vector &operator=(vector &&other) noexcept {
            _swap_vectors(*this, other);
        }

        friend void _swap_vectors(vector &v1, vector &v2) {
            using std::swap;
            swap(v1._size, v2._size);
            swap(v1._last, v2._last);
            swap(v1._capacity, v2._capacity);
            swap(v1._entries, v2._entries);
        }

    };
}

#endif //MY_VECTOR_H
