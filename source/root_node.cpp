// #include "root_node.hpp"
// #include "mvector.hpp"
// #include <iostream>
// 
// using std::cout;
// using std::endl;
// using mstd::vector;
// 
// 
// /*
//  * root node implementation
//  */
// root_node::root_node(size_t initial_size) : _children(initial_size) { }
// 
// trie_node *root_node::add_child(std::string &word, bool eow) {
//     return _children.insert(word, eow);
// }
// 
// void root_node::remove_child(int index) {
// 
// }
// 
// trie_node *root_node::get_child(const std::string &word, int *at = nullptr) {
//     return _children.get(word);
// }
