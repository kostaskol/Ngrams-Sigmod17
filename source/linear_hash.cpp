#include "linear_hash.hpp"
#include "binary_search.hpp"
#include "logger.hpp"
#include "helpers.hpp"
#include <iostream>
#include <cstring>
#include <sstream>


using mstd::vector;
using mstd::logger;
using std::string;
using std::to_string;
using std::stringstream;
using std::cout;
using std::endl;
using std::cerr;

template <typename T>


linear_hash<T>::linear_hash(size_t initial_size) : _size(initial_size), _num_items(0), _p(0) {
    _entries = new vector<T>*[_size];
    for (int i = 0; i < initial_size; i++) {
        _entries[i] = new vector<T>(constants::LH_MAX_BUCKET_SIZE);
    }
}

template <typename T>
linear_hash<T>::~linear_hash() {
    for (int i = 0; i < _size + _p; i++) {
        delete _entries[i];
    }
    delete[] _entries;
}

template <typename T>
T *linear_hash<T>::insert(string &word, bool eow) {
    int hash;
    int inner_index;
    // Use linear_hash::get to check if the entry already exists...
    T *entry = get(word, &hash, &inner_index);

    // ...in which case, simply return it
    if (entry != nullptr) return entry;

    // otherwise, insert it
    int load = _calculate_load();

    bool split = false;
    if (load > constants::LH_LOAD_FACTOR) {
        split = true;
        // Load factor has been reached. Split at _p

        vector<T> *tmp_storage = _entries[_p]; // Keep a temporary storage

        _resize(); // Allocate one more bucket at the end of the table

        _entries[_p] = new vector<T>(constants::LH_MAX_BUCKET_SIZE); // And create a new bucket

        // Rehash each element in tmp_storage to either _p or _size + _p
        if (tmp_storage != nullptr) {
            for (int i = 0; i < tmp_storage->size(); i++) {
                string tmp_word = tmp_storage->at(i).get_word();
                int hash = _hash(tmp_word);

                int index = hash % (2 * _size);
                int child_index;
                if (index > _size + _p) {
                    logger::warn("linear_hash::insert", "index (" + to_string(index) + ") > current size (" +
                                                        to_string(_size + _p) + "). Size = " + to_string(_size)
                                                        + "\tHash: " +
                                                        to_string(hash) + " word = " + tmp_word);
                }
                if (_entries[index] == nullptr) {
                    logger::error("linear_hash::insert", "_entries[" + std::to_string(index) + "] was "
                                                                                                    "null.Terminating");
                    exit(-1);
                }
                if (!bsearch_children(tmp_word, *_entries[index], &child_index)) {
                    _entries[index]->m_insert_at(child_index, tmp_storage->at(i));
                } else {
                    // Sanity check
                    logger::error("linear_hash::insert", "Entry already existed within bucket after bucket split");
                }
            }

            // Delete the temporary object
            delete tmp_storage;
        }

        // Increase p after the split operation
        if ((++_p) == _size) {
            // Next hashing phase
            // If the hash table has doubled in size, reset p and double the size
            _size *= 2;
            _p = 0;
        }
    }


    size_t index = hash % _size;
    if (index < _p) {
        index = hash % (2 * _size);
    }

    T new_node(word, eow);

    T *ret = nullptr;

    vector<T> *v = _entries[index];

    if (!split) {
        // If the bucket hasn't split
        // We can use the vector's index provided by linear_hash::get
        ret = v->m_insert_at(inner_index, new_node);
        _num_items++;
    } else {
        // Otherwise, we need to search the bucket again
        int child_index;
        if (!bsearch_children(word, *v, &child_index)) {
            // Child does not exist, so we must add it at index <child_index>
            _num_items++;
            ret = v->m_insert_at(child_index, new_node);
        } else /* TODO: Is this a possibility? */{
            // Child exists. Simply return it
            logger::warn("linear_hash::insert", "Found that child exists during insertion and not search",
                         constants::LOGFILE);

            ret = v->get_p(child_index);
        }
    }

    return ret;
}

template <typename T>
T *linear_hash<T>::get(const std::string &word, int *hash, int *index) const {

    int mhash = _hash(word);
    if (hash != nullptr) {
        // The caller has requested the hash value
        *hash = mhash;
    }
    size_t mindex = mhash % _size;
    if (mindex < _p) {
        mindex = mhash % (_size * 2);
    }

    int child_index;
    T *ret = nullptr;
    if (bsearch_children(word, *_entries[mindex], &child_index)) {
        ret = &_entries[mindex]->at(child_index);
    }
    if (index != nullptr) {
        *index = child_index;
    }

    return ret;
}

template <typename T>
T *linear_hash<T>::get_static(const std::string &word) const {
    int hash = _hash(word);
    size_t index = hash % _size;
    if (index < _p) {
        index = hash % (_size * 2);
    }

    return static_bsearch(word, *_entries[index]);
}

template <typename T>
void linear_hash<T>::delete_word(const std::string &word) {
    int hash = _hash(word);
    size_t index = hash % _size;
    if (index < _p) {
        index = hash % (_size * 2);
    }

    int child_index;
    if (bsearch_children(word, *_entries[index], &child_index)) {
        _entries[index]->remove_at((size_t) child_index);
        _num_items--;
    }
}

// FIXME: Should this return _size or _size + _p?
template <typename T>
size_t linear_hash<T>::size() const {
    return _size;
}


template <typename T>
void linear_hash<T>::push_to_stack(mstd::stack<T *> *s) const {
    for (int i = 0; i < _size + _p; i++) {
        for (int j = 0; j < _entries[i]->size(); j++) {
            s->push(_entries[i]->get_p((size_t)j));
        }
    }
}

template <typename T>
bool linear_hash<T>::empty() const {
    return _num_items == 0;
}


template <typename T>
int linear_hash<T>::_hash(const std::string &word) const {
    char *cont_str = new char[word.length() + 1];
    strcpy(cont_str, word.c_str());
    char *str = cont_str;
    int hash = 5381;
    int c;


    while (c = *str++)
        hash = ((hash << 5) + hash) + c;

    delete[] cont_str;

    return hash >= 0 ? hash : -hash;
}

template <typename T>
void linear_hash<T>::_resize() {
    auto tmp = new vector<T>*[_size + _p + 1];
    for (size_t i = 0; i < _size + _p; i++) {
        if (i == _p) continue; // No need to copy the bucket that is going to be split
        tmp[i] = _entries[i];
    }

    // Initialise the newly created bucket
    tmp[_size + _p] = new vector<T>(constants::LH_MAX_BUCKET_SIZE);

    delete[] _entries;
    _entries = tmp;
}

template <typename T>
int linear_hash<T>::_calculate_load() const {
    return (int) ((_num_items + 1) / (double) ((_size + _p) * constants::LH_MAX_BUCKET_SIZE) * 100);
}

template class linear_hash<trie_node>;
template class linear_hash<static_node>;