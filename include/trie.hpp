#ifndef TRIE
#define TRIE

#include <string>
#include "mvector.hpp"
#include "logger.hpp"

class trie {
private:
    class trie_node {
    private:
        std::string _word;
        bool _eow;
        mstd::vector<trie_node> *_children;
        trie_node *_parent;
    public:
        trie_node();

        trie_node(const std::string &word, bool eow, trie_node *par = nullptr);

        trie_node(const trie_node &other);

        trie_node(trie_node &&other) noexcept;

        ~trie_node();

        trie_node *add_child(std::string word, bool eow);

        const mstd::vector<trie_node> &get_children();

        trie_node *get_child(int index);

        trie_node *get_child(std::string &word);

        void push_child(trie_node *node);

        bool is_end_of_word();

        size_t children_size();

        std::string get_word();

        void set_end_of_word();

        trie_node &operator=(const trie_node &other);

        // Move assignment operator
        // "Steals" the _children pointer from the other
        // trie_node
        trie_node &operator=(trie_node &&other) noexcept;
    };

    trie_node *_root;
    size_t _size;
public:
    trie();
    ~trie();

    void add(const mstd::vector<std::string> &ngram);

    bool search(const mstd::vector<std::string> &ngram);
};

#endif // TRIE
