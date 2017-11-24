// #ifndef TRIE_NODES
// #define TRIE_NODES
// 
// #include "mvector.hpp"
// #include "constants.hpp"
// #include "linear_hash.hpp"
// #include <string>
// 
// 
// class root_node {
// public:
//     explicit root_node(size_t initial_size = LINEAR_HASH_INITIAL_SIZE);
// 
//     ~root_node()=default;
// 
//     trie_node *add_child(std::string &word, bool eow);
// 
//     void remove_child(int index);
// 
//     trie_node *get_child(const std::string &word);
// private:
//     linear_hash _children;
// };
// 
// #endif
