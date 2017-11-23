#ifndef TRIES_LINEAR_HASH_HPP
#define TRIES_LINEAR_HASH_HPP
#include <string>
#include "mvector.hpp"
#include "trie_node.hpp"
#include "constants.hpp"


class linear_hash {
public:
    explicit linear_hash(size_t initial_size = LINEAR_HASH_INITIAL_SIZE);

    ~linear_hash();

    trie_node *insert(std::string &word, bool eow);

    trie_node *get(const std::string &word);

    void delete_word(const std::string &word);

    size_t size();
    
    void print();
private:
    size_t _size;
    size_t _num_items;
    size_t _p;

    mstd::vector<trie_node> **_entries;
    
    size_t _hash(const std::string &word);
    
    void _resize();
};


#endif //TRIES_LINEAR_HASH_HPP
