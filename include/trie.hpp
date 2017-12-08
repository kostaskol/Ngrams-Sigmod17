#ifndef TRIE
#define TRIE

#include <string>
#include "mvector.hpp"
#include "logger.hpp"
#include "mqueue.hpp"
#include "hash_table.hpp"
#include "trie_nodes.hpp"


class trie {
protected:
    size_t _num_nodes;

    size_t _num_ngrams;

private:
    root_node *_root;

public:
    trie();

    virtual ~trie();

    virtual void add(const mstd::vector<std::string> &ngram);

    virtual void search(const mstd::vector<std::string> &ngram, mstd::queue<std::string> *results);

    virtual void compress();

    bool delete_ngram(const mstd::vector<std::string> &ngram);

    friend std::ostream &operator<<(std::ostream &out, const trie_node &other);
};

class static_trie : public trie {
private:
//    static_node *_root;
    static_root_node*_root;
public:
    static_trie();

    ~static_trie() override;

    void add(const mstd::vector<std::string> &ngram) override;

    void search(const mstd::vector<std::string> &ngram, mstd::queue<std::string> *results) override;

    void compress() override;

    size_t get_num_nodes();

    size_t get_num_ngrams();
};

#endif // TRIE
