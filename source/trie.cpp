#include "trie.h"
#include "logger.h"

#include <iostream>

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

void trie::add(mstd::string ngram) {
    trie_node *current = _root;
    mstd::vector<mstd::string> grams = ngram.split(' ');
    for (int i = 0; i < grams.size() - 1; i++) {
        // TODO: Binary search here
        if (!current->has_child(grams.at(i))) {
            current = current->add_child(grams.at(i), false);
        } else {
            current = current->get_child(grams.at(i));
            if (current == nullptr) {
                mstd::logger::error("trie::add", "current is null");
                throw std::runtime_error("current is null");
            }
        }
    }

    mstd::string last_word = grams.get((int) grams.size() - 1);

    if (current->has_child(last_word)) {
        cout << "Last word " << last_word << " existed in the tree" << endl;
        current->set_end_of_word();
    } else {
        cout << "Adding last child: " << last_word << endl;
        current->add_child(last_word, true);
    }

}

bool trie::search(mstd::string ngram) {
    trie_node *current = _root;
    mstd::vector<mstd::string> grams = ngram.split(' ');
    for (int i = 0; i < grams.size() - 1; i++) {
        if (!current->has_child(grams.get(i))) {
            return false;
        }
        current = current->get_child(grams.get(i));
    }

    if ((current = current->get_child(grams.get((int) grams.size() - 1))) != nullptr) {
        return current->is_end_of_word();
    }
}
