#include "trie.h"
#include "logger.h"

#include <iostream>
#include <sstream>
#include "helpers.h"

using namespace std;


trie::trie() {
    _root = new trie_node();
    _size = 0;
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

void trie::add(const std::string &ngram) {
    // Start at the root
    trie_node *current = _root;


    // Get the parts of the N-Gram
    mstd::vector<std::string> grams;
    std::stringstream ss(ngram);
    std::string tmp;
    while (std::getline(ss, tmp, ' ')) {
        grams.add(tmp);
    }

    // Go up until the previous to last part (we need to treat the last part differently)
    for (int i = 0; i < grams.size() - 1; i++) {
        // TODO: Binary search here
        trie_node *child;
        if ((child = current->get_child(grams.at(i))) == nullptr) {
            // If the current trie_node doesn't already contain that child, add it (not as an end of word)
            current = current->add_child(grams.at(i), false);
        } else {
            // Otherwise, follow that child's path
            current = child;
        }
    }

    std::string last_word = grams.get_cpy((int) grams.size() - 1);

    if (current->get_child(last_word) != nullptr) {
        // If the word already existed in the tree, we simply mark it as the end of the N-Gram
        current->set_end_of_word();
    } else {
        // Otherwise we insert it to that node's children
        current->add_child(last_word, true);
    }
}

bool trie::search(const std::string &ngram) {
    trie_node *current = _root;
    mstd::vector<std::string> grams;
    helpers::split(ngram, grams, ' ');
    for (int i = 0; i < grams.size() - 1; i++) {
        if (current->get_child(grams.get(i)) == nullptr) {
            return false;
        }
        current = current->get_child(grams.get(i));
    }

    return ((current = current->get_child(grams.get((int) grams.size() - 1))) != nullptr && current->is_end_of_word());
}
