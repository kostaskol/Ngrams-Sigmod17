#ifndef TRIE
#define TRIE

#include <string>
#include "mvector.hpp"
#include "logger.hpp"
#include "mqueue.hpp"
#include "mstack.hpp"
#include "hash_table.hpp"


class trie {
protected:
    class trie_node {
    protected:
        std::string _word;

        bool _eow;

        mstd::vector<trie_node> *_children;

        bool _bsearch_children(std::string &word, int *index);

    public:
        trie_node();

        trie_node(const std::string &word, bool eow);

        trie_node(const trie_node &other)=delete;

        trie_node(trie_node &&other) noexcept;

        virtual ~trie_node();

        virtual trie_node *add_child(int index, std::string word, bool eow);

        void remove_child(int index);

        mstd::vector<trie::trie_node> *get_children_p();

        virtual trie_node *get_child(int index);

        virtual trie_node *get_child(std::string &word, int *at);

        bool is_end_of_word() const;

        std::string& get_word();

        void set_word(std::string word);

        virtual void add_short(std::string &word, bool eow);

        void set_end_of_word(bool v);

        void set_children(mstd::vector<trie::trie_node> *c);

        size_t get_children_size();

        void push_children(mstd::stack<trie::trie_node *> *s);

        void print(int level);

        void to_string(std::stringstream &ss, int level);

        trie_node &operator=(const trie_node &other);

        // Move assignment operator
        // "Steals" the _children pointer from the other
        // trie_node
        trie_node &operator=(trie_node &&other) noexcept;

        friend std::ostream &operator<<(std::ostream &out, const trie_node &other);

    };

    class static_node : public trie_node {
    private:
        mstd::vector<signed short> *_lenofwords;

    public:
        static_node();

        static_node(const std::string &word, bool eow);

        ~static_node() override;

        static_node *add_child(int index, std::string word, bool eow) override;

        static_node *get_child(int index) override;

        static_node *get_child(std::string &word, int *at) override;

        mstd::vector<signed short> *get_lenofwords_p();

        void add_short(std::string &word, bool eow) override;

        static_node &operator=(const static_node &other);

        static_node &operator=(trie::static_node &&other) noexcept;

    };

    size_t _num_nodes;
    size_t _num_ngrams;

private:
    trie_node *_root;

    bool r_delete_helper(const mstd::vector<std::string> &ngram, trie_node *current, int length, int level, int *found);

public:
    trie();

    virtual ~trie();

    virtual void add(const mstd::vector<std::string> &ngram);

    virtual void search(const mstd::vector<std::string> &ngram, mstd::queue<std::string> *results);

    virtual void compress();

    bool delete_ngram(const mstd::vector<std::string> &ngram);

    bool r_delete_ngram(const mstd::vector<std::string> &ngram);

    friend std::ostream &operator<<(std::ostream &out, const trie_node &other);

    virtual void print_tree();

    virtual std::string to_string();

};

class static_trie : public trie {
private:
    static_node *_root;


public:
    static_trie();

    ~static_trie() override;

    void add(const mstd::vector<std::string> &ngram) override;

    //void search(const mstd::vector<std::string> &ngram, mstd::queue<std::string> *results) override;

    void compress() override;

    void print_tree() override;

    std::string to_string() override;

};

#endif // TRIE
