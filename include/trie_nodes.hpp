#ifndef TRIE_NODES
#define TRIE_NODES
#include "mvector.hpp"
#include "linear_hash.hpp"
#include <string>


class trie_node {
protected:
    std::string _word;
    bool _eow;
private:
    mstd::vector<trie_node> *_children;
public:
    trie_node();

    trie_node(const std::string &word, bool eow);

    trie_node(const trie_node &other)=delete;

    trie_node(trie_node &&other) noexcept;

    virtual ~trie_node();

    virtual trie_node *add_child(std::string &word, bool eow, int index);

    void remove_child(int index);

    mstd::vector<trie_node> *get_children_p();

    virtual trie_node *get_child(int index);

    virtual trie_node *get_child(const std::string &word, int *at);

    bool is_end_of_word() const;

    virtual const std::string& get_word();

    virtual std::string get_word(int index);

    void set_word(const std::string &word);

    virtual bool is_root() const;

    virtual bool has_children() const;

    virtual void add_short(const std::string &word, bool eow);

    void set_end_of_word(bool v);

    void to_string(std::stringstream &ss, int level);

    virtual void print(int level);

    trie_node &operator=(const trie_node &other);

    // Move assignment operator
    // "Steals" the _children pointer from the other
    // trie_node
    trie_node &operator=(trie_node &&other) noexcept;

    friend std::ostream &operator<<(std::ostream &out, const trie_node &other);

};

class static_node : public trie_node {
private:
    mstd::vector<static_node> *_children;

protected:
    mstd::vector<signed short> *_lenofwords;

public:
    static_node();

    static_node(const std::string &word, bool eow);

    ~static_node() override;

    static_node *add_child(std::string &word, bool eow, int index) override;

    static_node *get_child(int index) override;

    static_node *get_child(const std::string &word, int *at) override;

    virtual static_node *search_static_child(const std::string &word);

    mstd::vector<static_node> *get_st_children_p();

    mstd::vector<signed short> *get_lenofwords_p();

    void steal_children(mstd::vector<static_node> *c);

    virtual void push_children(mstd::stack<static_node *> *s);

    size_t get_children_size();

    const std::string& get_word() override;

    bool end_of_word(int index);

    std::string get_word(int index) override;

    size_t lenofwords_size();

    void print_shorts();

    bool has_children() const override;

    void print(int level) override;

    void add_short(const std::string &word, bool eow) override;

    static_node &operator=(const static_node &other);

    static_node &operator=(static_node &&other) noexcept;

};

class root_node : public trie_node {
private:
    linear_hash<trie_node> _children;

public:

    explicit root_node(size_t initial_size = constants::LH_INIT_SIZE);

    ~root_node() override =default;

    trie_node *add_child(std::string &word, bool eow, int index) override;

    void remove_child(const std::string &word);

    trie_node *get_child(const std::string &word, int * at) override;

    bool has_children() const override;

    bool is_root() const override;

    bool empty();
};

class static_root_node : public static_node {
private:
    linear_hash<static_node> _children;

public:
    explicit static_root_node(size_t initial_size = constants::LH_INIT_SIZE);

    ~static_root_node() override = default;

    static_node *add_child(std::string &word, bool eow, int index) override;

    static_node *get_child(const std::string &word, int * at) override;

    static_node *search_static_child(const std::string &word) override;

    bool has_children() const override;

    bool empty();

    void push_children(mstd::stack<static_node *> *s) override;

};
#endif // TRIE_NODES