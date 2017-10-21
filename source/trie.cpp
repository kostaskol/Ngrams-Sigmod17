#include "trie.hpp"
#include "logger.hpp"

#include <iostream>
#include <sstream>
#include "helpers.hpp"

using namespace std;

/*
 * Trie Implementation
 */
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
    std::cout << "LALA" << '\n';
    // Go up until the previous to last part (we need to treat the last part differently)
    for (int i = 0; i < grams.size() - 1; i++) {
        trie_node *child;
        if ((child = current->get_child(grams.at(i))) == nullptr) {
            std::cout << "POUTSES" << '\n';
            // If the current trie_node doesn't already contain that child, add it (not as an end of word)
            current = current->add_child(grams.at(i), false);
            std::cout << "Prosthesa to paidi : " << current->get_word() <<'\n';
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
        std::cout << "PATERAS " << current->get_word() << '\n';
        // Otherwise we insert it to that node's children
        current = current->add_child(last_word, true);
        std::cout << "Prosthesa to paidi : " << current->get_word() <<'\n';
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


/*
 * trie_node implementation
 */

trie::trie_node::trie_node()
        : _word(""), _eow(false), _parent(nullptr) {
    _children = nullptr;
}

trie::trie_node::trie_node(const std::string &word, bool eow, trie_node *par)
        : _word(word), _eow(eow), _parent(par) {
    _children = nullptr;
}

trie::trie_node::trie_node(const trie_node &other)
        : _word(other._word), _eow(other._eow), _parent(other._parent) {
    if (other._children == nullptr) {
        _children = nullptr;
        return;
    }
    _children = new mstd::vector<trie_node>(*other._children);
}

trie::trie_node::trie_node(trie_node &&other) noexcept {
    _word = other._word;
    _eow = other._eow;
    _parent = other._parent;
    _children = other._children;
    other._children = nullptr;
}

trie::trie_node::~trie_node() {
    delete _children;
}

trie::trie_node *trie::trie_node::add_child(std::string word, bool eow) {
    int index;
    if (_children == nullptr) {
        _children = new mstd::vector<trie_node>();
    }
    trie_node new_node(word, eow, this);
    if (_children->size() == 0) {
        _children->m_push(new_node);
    } else {
        //TODO: remove for loop and replace with bsearch.
        // for (int i = 0; i < (size_t) _children->size(); i++) {
        //     if (word > _children->get(i)._word) {
        //         _children->m_insert_at(i, new_node);
        //         break;
        //     }
        // }
        if (!_bsearch_children(word,&index)) {
            std::cout << "SUNOLIKA PAIDIA " << _children->size() <<'\n';
            std::cout << "MPHKA ME " << index <<'\n';
            _children->m_insert_at(index, new_node);
            return _children->at_p(index);
        }
        else{
            return nullptr; //if we are careful to call add_child only when the child we are adding doesnt not already exists, this line is not usefull
        }
    }
    return _children->get_last_inserted();
}

const mstd::vector<trie::trie_node> &trie::trie_node::get_children() {
    return *_children;
}

trie::trie_node *trie::trie_node::get_child(int index) {
    return _children->at_p(index);
}

trie::trie_node *trie::trie_node::get_child(std::string &word) {
    if (_children == nullptr) return nullptr;
    int index;
    if(!_bsearch_children(word,&index)){
        return nullptr;
    }
    else{
        return get_child(index);
    }

    // for (int i = 0; i < _children->size(); i++) {
    //     if (_children->at(i)._word == word) {
    //         return _children->at_p(i);
    //     }
    // }
    // return nullptr;
}


bool trie::trie_node::_bsearch_children(std::string &word, int *index){
    bool found = false;
    int left = 0;
    int right = _children->size() - 1;
    while (left <= right) {
        int mid = left + ((right-left) / 2);
        if (_children->at(mid)._word == word) {
            found = true;
            *index = mid;
            return found;
        }

        if (left == right) {    //mid._word != word here, so we return where the new word should be added.
            if (_children->at(left)._word.compare(word) > 0) {
                *index = left+1;
            }
            else{
                *index = left;
            }
            return found;
        }

        if (_children->at(mid)._word.compare(word) < 0) {
            right = mid - 1;
        }
        else if (_children->at(mid)._word.compare(word) > 0) {
            left = mid + 1;
        }

    }
}

void trie::trie_node::push_child(trie_node *node) {
    if (_children == nullptr) {
        _children = new mstd::vector<trie_node>();
    }
    _children->push(*node);
}

bool trie::trie_node::is_end_of_word() {
    return _eow;
}

size_t trie::trie_node::children_size() {
    return _children->size();
}

std::string trie::trie_node::get_word() {
    return _word;
}

void trie::trie_node::set_end_of_word() {
    _eow = true;
}

trie::trie_node &trie::trie_node::operator=(const trie::trie_node &other) {
    _word = other._word;
    _eow = other._eow;
    _children = new mstd::vector<trie_node>(*other._children);
    _parent = other._parent;
    return *this;
}

// Move assignment operator
// "Steals" the _children pointer from the other
// trie_node
trie::trie_node &trie::trie_node::operator=(trie::trie_node &&other) noexcept {
    _word = other._word;
    _eow = other._eow;
    _children = other._children;
    other._children = nullptr;
    _parent = other._parent;
    return *this;
}
