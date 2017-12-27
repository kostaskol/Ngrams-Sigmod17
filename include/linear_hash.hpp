#ifndef TRIES_LINEAR_HASH_HPP
#define TRIES_LINEAR_HASH_HPP
#include <string>
#include "mvector.hpp"
#include "constants.hpp"
#include "mstack.hpp"
#include "pair.hpp"

class static_node;
class trie_node;
template <typename T>
class linear_hash {
public:
    explicit linear_hash(size_t initial_size = constants::LH_INIT_SIZE);

    ~linear_hash();

    T *insert(T &new_node);

    // If we're searching for plain trie nodes, we use bsearch_children
    T *get(const std::string &word, int *hash = nullptr, int *index = nullptr) const;

    // If we're searching for static nodes, we use static_bsearch
    static_node *get_static(const std::string &word) const;

    void delete_word(const std::string &word);

    size_t get_num_items() const;

    T *next_branch();

    void reset_branch();

    size_t size() const;

    void push_to_stack(mstd::stack<T*> *s) const;

    bool empty() const;

    void print_table() {
        for (int i = 0; i < _size + _p; i++) {
            for (int j = 0; j < _entries[i]->size(); j++) {
                std::cout << _entries[i]->get(j).get_word() << std::endl;
            }
        }
    }
private:
    // We do not keep a <current size> variable, since we can always calculate it as the
    // size of the table at the start of each run + p (the next bucket to be split)
    size_t _size;
    size_t _num_items;
    size_t _p;

    size_t _current_branch;
    size_t _current_bucket;

    mstd::vector<T> **_entries;

    pthread_mutex_t _del_mtx;

    int _hash(const std::string &word) const;

    void _resize();

    int _calculate_load() const;
};

/*------------------------------------------------------------------------------------------------------------------*/

class linear_hash_int {
public:
    explicit linear_hash_int(size_t initial_size = constants::LH_INIT_SIZE);

    ~linear_hash_int();

    pair *insert(std::string &word);

    pair *get(const std::string &word, int *hash = nullptr, int *index = nullptr) const;

    void delete_word(const std::string &word);

    size_t size() const;

    void print() const;

    std::string stats(bool v = true) const;

    bool empty() const;

    size_t get_max() const;

    void fill_with_items(mstd::vector<pair> *array);
private:
    size_t _size;
    size_t _num_items;
    size_t _p;
    size_t _max;

    mstd::vector<pair> **_entries;

    int _hash(const std::string &word) const;

    void _resize();

    int _calculate_load() const;
};

#endif //TRIES_LINEAR_HASH_HPP
