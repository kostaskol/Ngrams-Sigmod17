#ifndef TRIES_LINEAR_HASH_HPP
#define TRIES_LINEAR_HASH_HPP
#include <string>
#include "mvector.hpp"
#include "constants.hpp"

class trie_node;

class linear_hash {
public:
    explicit linear_hash(size_t initial_size = LINEAR_HASH_INITIAL_SIZE);

    ~linear_hash();

    trie_node *insert(std::string &word, bool eow);

    trie_node *get(const std::string &word) const;

    void delete_word(const std::string &word);

    size_t size() const;

    void print() const;

    std::string stats(bool v = true) const;

    bool empty() const;
private:
    size_t _size;
    size_t _num_items;
    size_t _p;

    mstd::vector<trie_node> **_entries;

    int _hash(const std::string &word) const;

    void _resize();

    int _calculate_load() const;
};


#endif //TRIES_LINEAR_HASH_HPP