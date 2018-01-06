#include "linear_hash.hpp"
#include "binary_search.hpp"
#include "helpers.hpp"
#include <iostream>
#include <cstring>
#include <sstream>


using mstd::vector;
using std::string;
using std::to_string;
using std::stringstream;
using std::cout;
using std::endl;

template <typename T>
linear_hash<T>::linear_hash(size_t initial_size) : _size(initial_size),
                                                   _num_items(0),
                                                   _p(0),
                                                   _current_branch(0),
                                                   _current_bucket(0) {
    pthread_mutex_init(&_del_mtx, nullptr);
    _entries = new vector<T>*[_size];
    for (size_t i = 0; i < initial_size; i++) {
        _entries[i] = new vector<T>(constants::LH_MAX_BUCKET_SIZE);
    }
}

template <typename T>
linear_hash<T>::~linear_hash() {
    for (size_t i = 0; i < _size + _p; i++) {
        delete _entries[i];
    }
    delete[] _entries;
}

template <typename T>
T *linear_hash<T>::insert(T &new_node) {
    int hash;
    int inner_index;
    // Use linear_hash::get to check if the entry already exists...
    T *entry = get(new_node.get_word(), &hash, &inner_index);

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
            for (int i = 0; i < (int) tmp_storage->size(); i++) {
                string tmp_word = tmp_storage->at(i).get_word();
                int hash = _hash(tmp_word);

                int index = hash % (2 * _size);
                int child_index;
                if (!bsearch_children(tmp_word, *_entries[index], &child_index)) {
                    _entries[index]->m_insert_at(child_index, tmp_storage->at(i));
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
        if (!bsearch_children(new_node.get_word(), *v, &child_index)) {
            // Child does not exist, so we must add it at index <child_index>
            _num_items++;
            ret = v->m_insert_at(child_index, new_node);
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
        ret = &_entries[mindex]->at((size_t) child_index);
    }
    if (index != nullptr) {
        *index = child_index;
    }

    return ret;
}

// template <typename T>
template <>
static_node *linear_hash<static_node>::get_static(const std::string &word) const {
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

    pthread_mutex_lock(&_del_mtx);
    if (bsearch_children(word, *_entries[index], &child_index)) {
        _entries[index]->remove_at((size_t) child_index);
        _num_items--;
    }
    pthread_mutex_unlock(&_del_mtx);
}


template <typename T>
size_t linear_hash<T>::get_num_items() const {
    return _num_items;
}

template <typename T>
T **linear_hash<T>::get_top_branches(int *size) {
    int current_branch = 0;
    int current_bucket = 0;
    *size = _num_items;

    T **ret = new T*[_num_items];

    for (int i = 0; i < (int) _num_items; i++) {
        while (current_branch >= (int) _entries[current_bucket]->size()) {
            current_bucket++;
            current_branch = 0;
        }
        ret[i] = _entries[current_bucket]->get_p(current_branch++);
    }

    return ret;
}

template <typename T>
size_t linear_hash<T>::size() const {
    return _size + _p;
}


template <typename T>
void linear_hash<T>::push_to_stack(mstd::stack<T *> *s) const {
    for (int i = 0; i < (int) (_size + _p); i++) {
        for (int j = 0; j < (int) _entries[i]->size(); j++) {
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


    while ((c = *str++))
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

/*-------------------------------------------------------------------------------------------------------------------------*/


linear_hash_int::linear_hash_int(size_t initial_size) : _size(initial_size), _num_items(0), _p(0), _max(0) {
    _entries = new vector<pair>*[_size];
    for (int i = 0; i < initial_size; i++) {
        _entries[i] = new vector<pair>(constants::LH_MAX_BUCKET_SIZE);
    }
}

linear_hash_int::~linear_hash_int() {
    for (int i = 0; i < _size + _p; i++) {
        delete _entries[i];
    }
    delete[] _entries;
}

pair *linear_hash_int::insert(string &word, size_t counted) {
    int hash;
    int inner_index;
    size_t tmp;

    pair *entry = get(word, &hash, &inner_index);

    if (entry != nullptr) {
        entry->count(counted);
        if ((tmp = entry->get_freq()) > _max) {
            _max = tmp;
        }
        return entry;
    }

    int load = _calculate_load();

    bool split = false;
    if (load > constants::LH_LOAD_FACTOR) {
        split = true;
        // Load factor has been reached. Split at _p

        vector<pair> *tmp_storage = _entries[_p]; // Keep a temporary storage

        _resize(); // Allocate one more bucket at the end of the table

        _entries[_p] = new vector<pair>(constants::LH_MAX_BUCKET_SIZE); // And create a new bucket

        // Allocate a new bucket at the end

        // Rehash each element in tmp_storage to either _p or _size + _p
        if (tmp_storage != nullptr) {
            for (int i = 0; i < tmp_storage->size(); i++) {
                string tmp_word = tmp_storage->at(i).get_word();
                int hash = _hash(tmp_word);

                int index = hash % (2 * _size);
                int child_index;

                if (!bsearch_children(tmp_word, *_entries[index], &child_index)) {
                    _entries[index]->m_insert_at(child_index, tmp_storage->at(i));
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
        // If the index is before the bucket pointer, that bucket has already been split
        // so we need to find a different index for it
        // rehash it to the next
        index = hash % (2 * _size);
    }
    pair *ret = nullptr;


    vector<pair> *v = _entries[index];

    // Perform binary search and insert the element

    if (!split) {
        // If the bucket hasn't split
        // We can use the vector's index provided by linear_hash::get
        pair new_node(word,counted); //New pair of <word,counted>
        ret = v->m_insert_at(inner_index, new_node);
        _num_items++;
    } else {
        // Otherwise, we need to search the bucket again
        int child_index;
        if (!bsearch_children(word, *v, &child_index)) {
            // Child does not exist, so we must add it at index <child_index>
            pair new_node(word,counted); //New pair of <word,counted>
            ret = v->m_insert_at(child_index, new_node);
            _num_items++;
        }
    }

    if ((tmp = ret->get_freq()) > _max) {
        _max = tmp;
    }

    return ret;
}

pair *linear_hash_int::get(const std::string &word, int *hash, int *index) const {
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
    pair *ret = nullptr;
    if (bsearch_children(word, *_entries[mindex], &child_index)) {
        return &_entries[mindex]->at(child_index);
    }
    if (index != nullptr) {
        *index = child_index;
    }

    return ret;
}

void linear_hash_int::delete_word(const std::string &word) {
    int hash = _hash(word);
    int index = hash % _size;
    if (index < _p) {
        index = hash % (_size * 2);
    }

    int child_index;
    if (bsearch_children(word, *_entries[index], &child_index)) {
        _entries[index]->remove_at(child_index);
        _num_items--;
    }
}

size_t linear_hash_int::size() const {
    return _size + _p;
}

void linear_hash_int::print() const {
    cout << "Size: " << _size << endl;
    cout << "Number of buckets: " << _size + _p << endl;
    cout << "Next split: " << _p << endl;
    for (int i = 0; i < _size + _p; i++) {
        cout << "Printing entry at index " << i << endl;
        for (int j = 0; j < _entries[i]->size(); j++) {
            // cout << _entries[i]->at(j).get_word() << "|";
            _entries[i]->at(j).print();
            std::cout << " | ";
        }

        cout << endl;
    }
}

string linear_hash_int::stats(bool v) const {
    stringstream ss;

    ss << "Number of buckets: " << _size + _p << "\n";
    ss << "Size of table: " << _size << "\n";
    ss << "Number of elements within table: " << _num_items << "\n";
    if (v) {
        ss << "Elements per bucket: \n";
    }
    size_t avg = 0;
    for (int i = 0; i < _size + _p; i++) {
        avg += _entries[i]->size();
        if (v) {
            ss << "Bucket #" << i << ": " << _entries[i]->size() << "\n";
        }
    }
    ss << "Average elements per bucket : " << ((double) avg / (_size + _p)) << "\n";

    return ss.str();
}


bool linear_hash_int::empty() const {
    return _num_items == 0;
}

size_t linear_hash_int::get_max() const {
    return _max;
}

void linear_hash_int::fill_with_items(mstd::vector<pair> *array){
    for (size_t i = 0; i < size(); i++) {
        for (size_t j = 0; j < _entries[i]->size(); j++) {

            pair *temp = _entries[i]->at_p(j);
            size_t freq = temp->get_freq();
            array[freq-1].m_push(*temp);
        }
    }
}

void linear_hash_int::merge(linear_hash_int &merged){
    for (size_t i = 0; i < size(); i++) {
        for (size_t j = 0; j < _entries[i]->size(); j++) {
            merged.insert(_entries[i]->at_p(j)->get_word(), _entries[i]->at_p(j)->get_freq());
        }
    }
}

int linear_hash_int::_hash(const std::string &word) const {
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

void linear_hash_int::_resize() {
    auto tmp = new vector<pair>*[_size + _p + 1];
    for (size_t i = 0; i < _size + _p; i++) {
        if (i == _p) continue; // No need to copy the bucket that is going to be split
        tmp[i] = _entries[i];
    }

    // Initialise the newly created bucket
    tmp[_size + _p] = new vector<pair>(constants::LH_MAX_BUCKET_SIZE);

    delete[] _entries;
    _entries = tmp;
}

int linear_hash_int::_calculate_load() const {
    return (int) ((_num_items + 1) / (double) ((_size + _p) * constants::LH_MAX_BUCKET_SIZE) * 100);
}


template class linear_hash<trie_node>;
template class linear_hash<static_node>;
