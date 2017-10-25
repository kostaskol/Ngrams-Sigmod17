#include "trie.hpp"
#include "logger.hpp"
#include "helpers.hpp"

using mstd::vector;
using std::string;
using std::cout;
using std::endl;
using mstd::logger;

#define SIZE 3
#define HASH_SIZE 10

/*
 * Trie Implementation
 */
trie::trie() : _num_ngrams(0), _num_nodes(0) {
    _root = new trie_node();
}

trie::~trie() {
    delete _root;
}

void trie::add(const vector<string> &ngram) {
    // Start at the root
    trie_node *current = _root;

    // Go up until the previous to last part (we need to treat the last part differently)
    for (int i = 0; i < ngram.size() - 1; i++) {
        trie_node *child;
        int index;
        if ((child = current->get_child(ngram.at(i), &index)) == nullptr) {
            // If the current trie_node doesn't already contain that child, add it (not as an end of word)
            _num_nodes++;
            current = current->add_child(index, ngram.at(i), false);
        } else {
            // Otherwise, follow that child's path
            current = child;
        }
    }

    std::string &last_word = ngram.get(ngram.size() - 1);
    trie_node *child;
    int index = 0;
    if ((child = current->get_child(last_word, &index)) != nullptr) {
        // If the word already existed in the tree, we simply mark it as the end of the N-Gram
        child->set_end_of_word(true);
    } else {
        // Otherwise we insert it to that node's children
        current->add_child(index, last_word, true);
        _num_nodes++;
    }
    _num_ngrams++;
}


void trie::search(const vector<string> &ngram, mstd::queue<std::string> *results) {
    trie_node *current = _root;
    std::stringstream ss , final_ss;
    mstd::hash_table<void *> ht;
    bool found_one = false;

    for (size_t i = 0; i < ngram.size(); i++) {
        trie_node *child;
        for (size_t j = i; j < ngram.size(); j++) {
            if ((child = current->get_child(ngram.at(j),nullptr)) == nullptr) {
                ss.str("");
                ss.clear();
                current = _root;
                break;
            }
            else{
                ss << ngram.at(j) + " ";
                if (child->is_end_of_word()) {
                    try {
                       // If the key cannot be found in the hash table, it throws an exception
                       // if it doesn't throw, it means that the key already exists (so we have already found the ngram)
                       // so we start from the beginning
                       ht.get(ss.str());
                       ss.str("");
                       ss.clear();
                       current = _root;
                       break;
                    } catch (std::exception &e) {
                       if (found_one) {
                           final_ss << "| ";
                       }
                       // If it throws, the key doesn't exist, so we add it to the hash table and continue
                       ht.put(ss.str(), nullptr);
                       // results->push(ss.str());
                       found_one = true;
                       final_ss << ss.str();
                    }
                }
                current = child;
            }
        }
    }
    results->push(final_ss.str());
    if (!found_one) {
        results->push("$$END$$");
    }
    ss.str("");
    ss.clear();
    final_ss.str("");
    final_ss.clear();
}

void trie::print_tree() {
    _root->print(0);
}

size_t trie::get_num_nodes() {
    return _num_nodes;
}

size_t trie::get_num_ngrams() {
    return _num_ngrams;
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

trie::trie_node *trie::trie_node::add_child(int index, std::string word, bool eow) {
    if (_children == nullptr) {
        _children = new mstd::vector<trie_node>(SIZE);
    }

    trie_node new_node(word, eow, this);
    return _children->m_insert_at(index, new_node);
}

const mstd::vector<trie::trie_node> &trie::trie_node::get_children() {
    return *_children;
}

trie::trie_node *trie::trie_node::get_child(int index) {
    return _children->at_p((size_t) index);
}

trie::trie_node *trie::trie_node::get_child(std::string &word, int *at) {
    // NOTE: Potential bug caught here.
    // Avoided simply because the variable passed by trie::add was initialised to 0
    // at was not properly set if the _children vector was null
    if (_children == nullptr) {
        if (at != nullptr) {
            *at = 0;
        }
        return nullptr;
    }
    int index;
    if(!_bsearch_children(word, &index)) {    // Not found
        if (at != nullptr) {
            *at = index;
        }
        return nullptr;
    } else {                                 // Found
        // If the child is found, we assign -1 to the "not found index" to avoid undefined behaviour
        if (at != nullptr) {
            *at = index;
        }
        return get_child(index);
    }
}

bool trie::trie_node::_bsearch_children(std::string &word, int *index) {
   if (_children->at(0)._word > word) {
       *index = 0;
       return false;
   }
   else if (_children->at(0)._word == word) {
       *index = 0;
       return true;
   }

   if (_children->at(_children->size() - 1)._word < word) {
       *index = (int) _children->size();
       return false;
   }
   else if (_children->at(_children->size() - 1)._word == word) {
       *index = _children->size() - 1;
       return true;
   }

   int left = 0;
   int right = (int) _children->size() - 1;
   while (left <= right) {
       int mid = left + ((right-left) / 2);

       if (_children->at((size_t) mid)._word == word) {
           *index = mid;
           return true;
       }

       if (left == right || left == right - 1) {
           if (_children->at((size_t) left)._word > word) {
               *index = left;
           }
           else if (_children->at((size_t) right)._word < word) {
               *index = right + 1;
           }
           else if (_children->at((size_t) right)._word > word) {
               *index = right;
           }
           else {
               *index = mid;
               return true;
           }
           return false;
       }
       if (_children->at(mid)._word > word) {
           right = mid - 1;
       }
       else if (_children->at(mid)._word < word) {
           left = mid + 1;
       }
    }
}

void trie::trie_node::push_child(trie_node *node) {
    if (_children == nullptr) {
        _children = new mstd::vector<trie_node>(SIZE);
    }
    _children->m_push(*node);
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

void trie::trie_node::set_end_of_word(bool v) {
    _eow = v;
}

void trie::trie_node::print(int level) {
    for (int i = 1; i < level; i++) {
        cout << "\t";
    }
    if (_word != "") {
        cout << "| ";
        cout << _word << endl;
    }
    if (_children == nullptr) return;
    for (size_t i = 0; i < _children->size(); i++) {
        _children->get(i).print(level + 1);
    }
}

trie::trie_node &trie::trie_node::operator=(const trie::trie_node &other) {
    _word = other._word;
    _eow = other._eow;
    if (other._children != nullptr) {
        delete _children;
        _children = new mstd::vector<trie_node>(*other._children);
    }
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

std::ostream &operator<<(std::ostream &out, const trie::trie_node &other) {
    out << other._word;
    return out;
}
