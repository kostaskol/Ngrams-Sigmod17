#ifndef TRIE
#define TRIE

#include <string>
#include "mvector.hpp"
#include "logger.hpp"
#include "mqueue.hpp"
#include "hash_table.hpp"


class trie {
private:
    class trie_node {
    private:
        std::string _word;
        bool _eow;
        mstd::vector<trie_node> *_children;
        trie_node *_parent;

        bool _bsearch_children(std::string &word, int *index);
    public:
        trie_node();

        trie_node(const std::string &word, bool eow, trie_node *par = nullptr);

        trie_node(const trie_node &other)=delete;

        trie_node(trie_node &&other) noexcept;

        ~trie_node();

        trie_node *add_child(int index, std::string word, bool eow);

        void remove_child(int index);

        const mstd::vector<trie_node> &get_children();

        mstd::vector<trie::trie_node> *get_children_p();

		trie_node *get_parent();

        trie_node *get_child(int index);

        trie_node *get_child(std::string &word, int *at);

        void push_child(trie_node *node);

        bool is_end_of_word() const;

        size_t children_size() const;

        std::string get_word() const;

        void set_end_of_word(bool v);

        void print(int level);

        void to_string(std::stringstream &ss, int level);

        const bool has_children();

        trie_node &operator=(const trie_node &other);

        // Move assignment operator
        // "Steals" the _children pointer from the other
        // trie_node
        trie_node &operator=(trie_node &&other) noexcept;

        friend std::ostream &operator<<(std::ostream &out, const trie_node &other);

    };

    trie_node *_root;
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

    size_t get_num_nodes();

    size_t get_num_ngrams();

    friend std::ostream &operator<<(std::ostream &out, const trie_node &other);

    void print_tree();

    std::string to_string();
};

#endif // TRIE
