#include "linear_hash.hpp"
#include "binary_search.hpp"
#include "logger.hpp"
#include <iostream>
#include <cstring>
#include <cmath>

class trie_node;

using mstd::vector;
using std::string;
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

    
    if ((((_num_items + 1) / (double) (_size + _p)) * 100) > LINEAR_HASH_LOAD_FACTOR) {
        // Load factor has been reached. Split at _p
        // Keep a temporary storage 
        // And create a new bucket 
        vector<trie_node> *tmp_storage = _entries[_p];
        _entries[_p] = new vector<trie_node>(LINEAR_HASH_MAX_BUCKET_SIZE);
        
        // Allocate a new bucket at the end
        _resize();
        
        // Rehash each element in tmp_storage to either _p or _size + _p
        if (tmp_storage != nullptr || tmp_storage->size() != 0) {
            for (int i = 0; i < tmp_storage->size(); i++) {
                string tmp_word = tmp_storage->at(i).get_word();
                int index = _hash(tmp_word) % (2 * _size);
                int child_index;
                if (_entries[index] == nullptr) {
                    exit(-1);
                }
                if (!bsearch_children(tmp_word, *_entries[index], &child_index)) {
                    _entries[index]->m_insert_at(child_index, tmp_storage->at(i));
                } else {
                    // Sanity check
    //                 mstd::logger::error("linear_hash::insert", "Entry already existed within bucket after bucket split");
                    cerr << "linear_hash::insert : Entry already existed within bucket right after bucket split!" << endl;
                }
            }
            
            // Delete the temporary object
            delete tmp_storage;
        } else {
            cerr << "tmp storage was null" << endl;
        }
        
         // Only increase size if the table has doubled
        if ((_size + _p + 1) == 2 * _size) {
            // Next hashing phase
            // Double the size and reset the _p
            _size *= 2;
            _p = 0;
        } else {
            _p++;
        }
    
    }
    
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


size_t linear_hash::_hash(const std::string &word) {
    char *cont_str = new char[word.length() + 1];
    strcpy(cont_str, word.c_str());
    char *str = cont_str;
    size_t hash = 5381;
    int c;
    

    while (c = *str++)
        hash = ((hash << 5) + hash) + c;
    
    delete[] cont_str;
    
    return hash > 0 ? hash : -hash;
}

// TODO: Figure out where it's best to increase _p
void linear_hash::_resize() {
    auto tmp = new vector<trie_node>*[_size + _p + 1];
    for (size_t i = 0; i < _size + _p; i++) {
//         if (i == _p) continue; // No need to copy the bucket that is going to be split
        tmp[i] = _entries[i];
    }
    
    // Initialise the newly created bucket
    tmp[_size + _p] = new vector<trie_node>(LINEAR_HASH_MAX_BUCKET_SIZE);
    
   
    delete[] _entries;
    _entries = tmp;
}
