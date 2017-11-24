#ifndef TRIE
#define TRIE

#include <string>
#include "mvector.hpp"
#include "logger.hpp"
#include "mqueue.hpp"
#include "hash_table.hpp"
// #include "root_node.hpp"
// #include "trie_node.hpp"
#include "trie_nodes.hpp"

class trie {
private:
    root_node *_root;
    size_t _num_nodes;
    size_t _num_ngrams;


    bool r_delete_helper(const mstd::vector<std::string> &ngram, trie_node *current, int length, int level, int *found);
public:
    trie();
    ~trie();

    void add(const mstd::vector<std::string> &ngram);

    void search(const mstd::vector<std::string> &ngram, mstd::queue<std::string> *results);

    bool delete_ngram(const mstd::vector<std::string> &ngram);

    bool r_delete_ngram(const mstd::vector<std::string> &ngram);

    friend std::ostream &operator<<(std::ostream &out, const trie_node &other);

    void print_tree();

    std::string to_string();
};

#endif // TRIE
