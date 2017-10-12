#ifndef TRIE
#define TRIE

#include "my_string.h"

class trie_node;

struct search_result {
    bool found;
    bool has_children;
};

class trie {
private:

    trie_node *_root;
    size_t _size;
    trie_node *_current;

    void _in_order(trie_node *root);
public:

    trie();
    ~trie();

    void add(my_string ngram);

    void reset();

    void print();

    bool search(my_string ngram);

    search_result search_one(my_string word);
};

#endif // TRIE