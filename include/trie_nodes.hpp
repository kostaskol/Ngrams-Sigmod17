#ifndef TRIE_NODES
#define TRIE_NODES
#include "mvector.hpp"
#include "linear_hash.hpp"
#include <string>


class trie_node {
public:
    trie_node();

    trie_node(const std::string &word, bool eow);

    trie_node(const trie_node &other)=delete;

    trie_node(trie_node &&other) noexcept;

    virtual ~trie_node();

    // Add a default index which root_node::add_child will ignore, but will allow
    // for polymorphism
    virtual trie_node *add_child(std::string &word, bool eow, int index = -1);

    void remove_child(int index = -1);

    mstd::vector<trie_node> *get_children_p();

    trie_node *get_child(int index);

    virtual trie_node *get_child(const std::string &word, int *at = nullptr);
    
    const std::string &get_word();

    bool is_end_of_word() const;

    void set_end_of_word(bool v);

    void print(int level);

    void to_string(std::stringstream &ss, int level);
    
    virtual bool has_children() const;
    
    virtual bool is_root() const;

    trie_node &operator=(const trie_node &other);

    // Move assignment operator
    // "Steals" the _children pointer from the other
    // trie_node
    trie_node &operator=(trie_node &&other) noexcept;

    friend std::ostream &operator<<(std::ostream &out, const trie_node &other);
private:
    std::string _word;
    bool _eow;
    mstd::vector<trie_node> *_children;

    bool _bsearch_children(std::string &word, int *index);
};

class root_node : public trie_node {
public:
    explicit root_node(size_t initial_size = LINEAR_HASH_INITIAL_SIZE);
    
    ~root_node()=default;
    
    trie_node *add_child(std::string &word, bool eow, int index = -1) override;
    
    void remove_child(const std::string &word);
    
    
    trie_node *get_child(const std::string &word, int * at = nullptr) override;
    
    void print();
    
    bool has_children() const override;
    
    bool is_root() const override;
    
    bool empty();
    
private:
    linear_hash _children;
};

#endif // TRIE_NODES
