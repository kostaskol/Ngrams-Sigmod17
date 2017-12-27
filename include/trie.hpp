#ifndef TRIE
#define TRIE

#include <string>
#include "mvector.hpp"
#include "logger.hpp"
#include "mqueue.hpp"
#include "trie_nodes.hpp"


class trie {
public:
    trie();

    virtual ~trie();

    void add(const mstd::vector<std::string> &ngram, int version = 0);

    std::string search(const mstd::vector<std::string> &ngram, int version);

    bool delete_ngram(const mstd::vector<std::string> &ngram, int version);

    void clean_up(trie_node *top);

    trie_node *next_branch();

    void reset_branch();
private:
    root_node *_root;
};

class static_trie : public trie {
public:
    static_trie();
    ~static_trie() override;

    void add(const mstd::vector<std::string> &ngram);

    std::string search(const mstd::vector<std::string> &ngram);

    void compress();

    void print_root() {
        _root->print();
    }
private:
    static_root_node*_root;
};

#endif // TRIE
