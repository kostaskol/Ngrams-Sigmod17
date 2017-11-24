#include "linear_hash.hpp"
#include "binary_search.hpp"
#include "logger.hpp"
#include "helpers.hpp"
#include <iostream>
#include <cstring>
#include <sstream>

class trie_node;

using mstd::vector;
using mstd::logger;
using std::string;
using std::to_string;
using std::stringstream;
using std::cout;
using std::endl;
using std::cerr;

linear_hash::linear_hash(size_t initial_size) : _size(initial_size), _num_items(0), _p(0) {
    _entries = new vector<trie_node>*[_size];
    for (int i = 0; i < initial_size; i++) {
        _entries[i] = new vector<trie_node>(LINEAR_HASH_MAX_BUCKET_SIZE);
    }
}

linear_hash::~linear_hash() {
    for (int i = 0; i < _size + _p; i++) {
        delete _entries[i];
    }
    delete[] _entries;
}

trie_node *linear_hash::insert(string &word, bool eow) {
    
    double load = _calculate_load();
    
    if (load > LINEAR_HASH_LOAD_FACTOR) {
        
        // Load factor has been reached. Split at _p

        vector<trie_node> *tmp_storage = _entries[_p]; // Keep a temporary storage 
        
        _resize(); // resize the hash table
        
        _entries[_p] = new vector<trie_node>(LINEAR_HASH_MAX_BUCKET_SIZE); // And create a new bucket 
        
        // Allocate a new bucket at the end
        
        // Rehash each element in tmp_storage to either _p or _size + _p
        if (tmp_storage != nullptr) {
            for (int i = 0; i < tmp_storage->size(); i++) {
                string tmp_word = tmp_storage->at(i).get_word();
                int hash = _hash(tmp_word);

                int index = hash % (2 * _size);
                int child_index;
                if (index > _size + _p) {
//                     logger::warn("linear_hash::insert", "index (" + to_string(index) + ") > current size (" + to_string(_size + _p) + "). Size = " + to_string(_size) + "\tHash: " + to_string(hash) + " word = " + tmp_word);
                }
                if (_entries[index] == nullptr) {
//                     logger::error("linear_hash::insert", "_entries[" + std::to_string(index) + "] was null. Terminating");
//                     exit(-1);
                }
                if (!bsearch_children(tmp_word, *_entries[index], &child_index)) {
                    _entries[index]->m_insert_at(child_index, tmp_storage->at(i));
                } else {
                    // Sanity check
                    logger::error("linear_hash::insert", "Entry already existed within bucket after bucket split");
//                     cerr << "linear_hash::insert : Entry already existed within bucket right after bucket split!" << endl;
                }
            }
            
            // Delete the temporary object
            delete tmp_storage;
        } else {
            cerr << "tmp storage was null" << endl;
        }
        
         // Increase p after the split operation
        if ((++_p) == _size) {
            // Next hashing phase
            // If the hash table has doubled in size, reset p and double the size
            _size *= 2;
            _p = 0;
//             logger::debug("linear_hash::insert", "Doubling size to " + to_string(_size) + " because we are over the load factor " + to_string(load));
        } else {
//             logger::debug("linear_hash::insert", "Increasing size to " + to_string(_size + _p + 1) + " because we are over the load factor " + to_string(load));
        }
    }
    
    trie_node new_node(word, eow);
    size_t hash = _hash(word);
    
    
    
    size_t index = hash % _size;
    if (index < _p) {
        // If the index is before the bucket pointer, that bucket has already been split
        // so we need to find a different index for it
        // rehash it to the next 
        index = hash % (2 * _size);
    }
    trie_node *ret = nullptr;

    
    
    vector<trie_node> *v = _entries[index];
    
    // Perform binary search and insert the element
    if (v == nullptr) {
        cerr << "v was null" << endl;
        exit(-1);
    }
    
    int child_index;
    if (!bsearch_children(word, *v, &child_index)) {
        // Child does not exist, so we must add it at index <child_index>
        _num_items++;
        ret = v->m_insert_at(child_index, new_node);
    } else {
        // Child exists. Simply return it
        ret = v->get_p(child_index);
    }
    
    if (ret == nullptr) throw std::runtime_error("Returning nullptr");
    
    return ret;
}

trie_node *linear_hash::get(const std::string &word) {
    int hash = _hash(word);
    int index = hash % _size;
    if (index < _p) {
        index = hash % (_size * 2);
    }
    
    int child_index;
    if (bsearch_children(word, *_entries[index], &child_index)) {
        return &_entries[index]->at(child_index);
    }
    return nullptr;
}

void linear_hash::delete_word(const std::string &word) {

}

size_t linear_hash::size() { 
    return _size; 
}

void linear_hash::print() {
    cout << "Size: " << _size << endl;
    cout << "Number of buckets: " << _size + _p << endl;
    cout << "Next split: " << _p << endl;
    for (int i = 0; i < _size + _p; i++) {
        cout << "Printing entry at index " << i << endl;
        for (int j = 0; j < _entries[i]->size(); j++) {
            cout << _entries[i]->at(j).get_word() << "|";
        }

        cout << endl;
    }
}

string linear_hash::stats(bool v) {
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

int linear_hash::_hash(const std::string &word) {
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

void linear_hash::_resize() {
    auto tmp = new vector<trie_node>*[_size + _p + 1];
    for (size_t i = 0; i < _size + _p; i++) {
        if (i == _p) continue; // No need to copy the bucket that is going to be split
        tmp[i] = _entries[i];
    }
    
    // Initialise the newly created bucket
    tmp[_size + _p] = new vector<trie_node>(LINEAR_HASH_MAX_BUCKET_SIZE);
    
    delete[] _entries;
    _entries = tmp;
}

double linear_hash::_calculate_load() {
    return (_num_items + 1) / (double) ((_size + _p) * LINEAR_HASH_MAX_BUCKET_SIZE) * 100;
}
