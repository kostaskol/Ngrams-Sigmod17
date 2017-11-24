// #ifndef TRIE_NODE_HPP
// #define TRIE_NODE_HPP
// 
// #include "mvector.hpp"
// #include <string>
// 
// class trie_node {
// public:
//     trie_node();
// 
//     trie_node(const std::string &word, bool eow);
// 
//     trie_node(const trie_node &other)=delete;
// 
//     trie_node(trie_node &&other) noexcept;
// 
//     ~trie_node();
// 
//     trie_node *add_child(int index, std::string word, bool eow);
// 
//     void remove_child(int index);
// 
//     mstd::vector<trie_node> *get_children_p();
// 
//     trie_node *get_child(int index);
// 
//     trie_node *get_child(std::string &word, int *at);
//     
//     const std::string &get_word();
// 
//     bool is_end_of_word() const;
// 
//     void set_end_of_word(bool v);
// 
//     void print(int level);
// 
//     void to_string(std::stringstream &ss, int level);
// 
//     trie_node &operator=(const trie_node &other);
// 
//     // Move assignment operator
//     // "Steals" the _children pointer from the other
//     // trie_node
//     trie_node &operator=(trie_node &&other) noexcept;
// 
//     friend std::ostream &operator<<(std::ostream &out, const trie_node &other);
// private:
//     std::string _word;
//     bool _eow;
//     mstd::vector<trie_node> *_children;
// 
//     bool _bsearch_children(std::string &word, int *index);
// };
// 
// #endif //TRIE_NODE_HPP
