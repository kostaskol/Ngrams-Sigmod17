#ifndef TRIE_NODES
#define TRIE_NODES
#include "mvector.hpp"
#include "linear_hash.hpp"
#include "pair.hpp"
#include <string>

class trie_node {
private:
    std::string _word;
    bool _eow;

    int _ver_added;
    int _ver_deleted;
    mstd::vector<trie_node> *_children;
public:
    trie_node();

    trie_node(const std::string &word, bool eow, int version = 0);

    trie_node(const trie_node &other)=delete;

    trie_node(trie_node &&other) noexcept;

    virtual ~trie_node();

    trie_node *add_child(std::string &word, bool eow, int index, int version = 0);

    void set_child_del_version(int index, int version);

    void delete_child(int index);

    virtual void delete_child(const std::string &w);

    virtual trie_node *get_child(int index);

    virtual trie_node *get_child(const std::string &word, int *at);

    bool is_end_of_word() const;

    const std::string& get_word() const;

    virtual bool has_children() const;

    virtual size_t get_children_size() const;

    void set_end_of_word(bool v);

    void set_add_version(int version);

    void set_del_version(int version);

    int get_add_version();

    int get_del_version();

    virtual void push_children(mstd::stack<trie_node *> *s);

    void push_children(mstd::stack<tuple<trie_node *, int>> *s);

    trie_node &operator=(const trie_node &other);

    // Move assignment operator
    // "Steals" the _children pointer from the other
    // trie_node
    trie_node &operator=(trie_node &&other) noexcept;
};


class root_node : public trie_node {
public:
    explicit root_node(size_t initial_size = constants::LH_INIT_SIZE);

    ~root_node() override =default;

    trie_node *add_child(std::string &word, bool eow, int version);

    void set_child_del_version(const std::string &word, int version);

    void delete_child(const std::string &word) override;

    trie_node *get_child(const std::string &word);

    void push_children(mstd::stack<trie_node *> *s) override;

    bool has_children() const override;

    size_t get_children_size() const override;

    trie_node *next_branch();

    bool empty();

private:
    linear_hash<trie_node> _children;
};


class static_node {
private:
    std::string _word;
    bool _eow;
    mstd::vector<static_node> *_children;
    mstd::vector<signed short> *_lenofwords;
protected:

public:
    static_node();

    static_node(const std::string &word, bool eow);

    virtual ~static_node();

    static_node *add_child(std::string &word, bool eow, int index);

    static_node *get_child(int index);

    static_node *get_child(const std::string &word, int *at);

    virtual static_node *search_static_child(const std::string &word);

    mstd::vector<static_node> *get_st_children_p();

    mstd::vector<signed short> *get_lenofwords_p();

    void steal_children(mstd::vector<static_node> *c);

    virtual void push_children(mstd::stack<static_node *> *s);

    size_t get_children_size();

    std::string get_word(int index);

    std::string get_word();

    void set_word(std::string word);

    void set_end_of_word(bool v);

    bool is_end_of_word();

    bool end_of_word(int index);

    size_t lenofwords_size();

    void print_shorts();

    virtual bool has_children() const;

    void add_short(const std::string &word, bool eow);

    static_node &operator=(const static_node &other);

    static_node &operator=(static_node &&other) noexcept;

};



class static_root_node : public static_node {
public:
    explicit static_root_node(size_t initial_size = constants::LH_INIT_SIZE);

    ~static_root_node() override = default;

    static_node *add_child(std::string &word, bool eow);

    static_node *get_child(const std::string &word);

    static_node *search_static_child(const std::string &word) override;

    bool has_children() const override;

    bool empty();

    void push_children(mstd::stack<static_node *> *s) override;

    void print() {
        _children.print_table();
    }
private:
    linear_hash<static_node> _children;
};
#endif // TRIE_NODES