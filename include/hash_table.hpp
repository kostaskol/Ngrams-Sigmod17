#ifndef TRIES_HASH_TABLE_H
#define TRIES_HASH_TABLE_H

#include <cstdio>
#include <cstring>
#include <string>
#include <mvector.hpp>

namespace mstd {

    template<typename B>
    class hash_table {
    private:
        template<typename V>
        struct hash_entry {
            std::string _key;
            V _value;
        };

        mstd::vector<hash_entry<B>> *_entries;
        size_t _size;

        int _hash_function(const std::string &key) const {
            auto *str = new char[key.length() + 1];
            auto *tmp = str;
            strcpy(str, key.c_str());
            int hash = 5381;
            int c;

            while ((c = *tmp++))
                hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

            // If we get a negative result, return -result
            delete[] str;
            return hash > 0 ? hash : -hash;

        }

    public:
        explicit hash_table(size_t size = 30) : _size(size) {
            _entries = new mstd::vector<hash_entry<B>>[size];
        }

        hash_table(const hash_table &)=delete;

        ~hash_table() {
            delete[] _entries;
        }

        void put(std::string key, B val) {
            hash_entry<B> tmp;
            tmp._key = key;
            tmp._value = val;
            size_t index = _hash_function(key) % _size;
            _entries[index].add(tmp);
        }

        B get(const std::string &key) const {
            size_t index = _hash_function(key) % _size;
            mstd::vector<hash_entry<B>> v = _entries[index];
            for (int i = 0; i < v.size(); i++) {
                if (v.get(i)._key == key) {
                    return v.get(i)._value;
                }
            }
            throw std::runtime_error("Unknown key");
        }
    };
}

#endif //TRIES_HASH_TABLE_H
