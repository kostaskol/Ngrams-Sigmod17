#include "trie.h"
#include <iostream>

using namespace std;


trie::trie() {
    _root = new trie_node();
    _size = 0;
    _current = _root;
}

trie::~trie() {
    delete _root;
}

void trie::_in_order(trie_node *root) {
    if (root == nullptr) return;
    for (int i = 0; i < root->children_size(); i++) {
        _in_order(root->get_child(i));
        cout << root->get_word() << endl;
    }
}

void trie::print() {
    _in_order(_root);
}

void trie::add(my_string ngram) {
    if (_current == nullptr) {
        _current = _root;
    }

    my_vector<my_string> grams = ngram.split(' ');
    for (int i = 0; i < grams.size() - 1; i++) {
        if (!_current->has_child(grams.at(i))) {
            _current = _current->add_child(grams.at(i), false);
        } else {
            _current = _current->get_child(grams.at(i));
            if (_current == nullptr) {
                throw std::runtime_error("(ERROR) ::=:: Something doesn't make sense (trie::add(my_string))");
            }
        }
    }
}

void trie::reset() {
    _current = _root;
}