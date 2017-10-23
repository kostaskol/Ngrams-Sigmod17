#include "trie.hpp"
#include "logger.hpp"
#include "helpers.hpp"

using mstd::vector;
using std::string;
using std::cout;
using std::endl;
using mstd::logger;

#define SIZE 3

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
            logger::debug("trie::add", "Current node's children: ");
            mstd::vector<trie_node> v = current->get_children();
            for (int i = 0; i < v.size(); i++) {
                logger::debug("trie::add", v[i].get_word());
            }
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
        if (index == -1) {
            mstd::logger::warn("trie::add", "get child's index is -1 even though the word was not found");
        }
        current->add_child(index, last_word, true);
        _num_nodes++;
    }
    _num_ngrams++;
}


// To be deleted
// TODO: Delete this after giannis pushes the proper search
bool trie::search(const vector<string> &ngram) {
    trie_node *current = _root;
    int tmp;
    for (int i = 0; i < ngram.size() - 1; i++) {
        if ((current = current->get_child(ngram.get(i), nullptr)) == nullptr) {
            return false;
        }
    }

    return ((current = current->get_child(ngram.get(ngram.size() - 1), &tmp))
            != nullptr && current->is_end_of_word());
}

// The following two functions *could* be used, but aren't
// TODO: Consider removing them along with trie::_num_nodes and trie::_num_ngrams
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

// Deleted copy constructor here since it isn't used anywhere (just as it should be)
// trie::trie_node::trie_node(const trie::trie_node &other)=delete;

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

    if (index < 0) {
        // If we want a fail fast method, uncomment the following line
        // throw std::runtime_error("Index below zero");
        // Otherwise, simply log it
        // and set index to a safe value
        logger::warn("trie::trie_node::add_child", "Index :" + std::to_string(index));
        index = 0;
    }

    if (index > _children->size()) {
        // If we want a fail fast method, uncomment the following line
        //throw std::runtime_error("Index above size (Index: " + std::to_string(index)
        //                         + " & Size: " + std::to_string(_children->size()) + ")");
        // Otherwise, simply log it
        // and set index to a safe value
        logger::warn("trie::trie_node::add_child", "Index above size (Index: " + std::to_string(index)
                                                   + " & Size: " + std::to_string(_children->size()) + ")");
        index = (int) _children->size();
    }

    trie_node new_node(word, eow, this);
    trie_node *tmp = _children->m_insert_at(index, new_node);
    logger::debug("trie::trie_node::add_child", "Current child's " + _word + " children:");
    for (size_t i = 0; i < _children->size(); i++) {
        logger::debug("trie::trie_node::add_child", _children->get(i)._word);
    }
    return tmp;
}

const mstd::vector<trie::trie_node> &trie::trie_node::get_children() {
    return *_children;
}

trie::trie_node *trie::trie_node::get_child(int index) {
    return _children->at_p((size_t) index);
}

const bool trie::trie_node::has_children() {
    return _children != nullptr;
}

trie::trie_node *trie::trie_node::get_child(std::string &word, int *at) {
    // NOTE: Potential bug caught here.
    // Avoided simply because the variable passed by trie::add was initialised to 0
    // at was not properly set if the _children vector was null
    if (_children == nullptr) {
        *at = 0;
        return nullptr;
    }
    int index;
    if(!_bsearch_children(word, &index)) {    // Not found
        if (at != nullptr) {
            *at = index;
        } else {
            // Not sure if <at> would ever be passed as nullptr after trie::search has been implemented
            // Just make sure to remove this if it *is* passed as nullptr
            // TODO(kostas): Delete these once the proper search has been implemented
            mstd::logger::warn("trie::trie_node::get_child", "index request variable was null");
        }
        return nullptr;
    } else {                                 // Found
        // If the child is found, we assign -1 to the "not found index" to avoid undefined behaviour
        if (at != nullptr) {
            *at = -1;
        } else {
            // Not sure if <at> would ever be passed as nullptr after trie::search has been implemented
            // Just make sure to remove this if it *is* passed as nullptr
            // TODO(kostas): Delete these once the proper search has been implemented
            mstd::logger::warn("trie::trie_node::get_child", "index request variable was null");
        }
        return get_child(index);
    }
}

bool trie::trie_node::_bsearch_children(std::string &word, int *index) {
    // If the word is less than the first element
    // or greater than the last, return the appropriate index without checking
   if (_children->at(0)._word > word) {
       *index = 0;
       return false;
   }

   if (_children->at(_children->size() - 1)._word < word) {
       *index = (int) _children->size();
       return false;
   }
    int left = 0;
    int right = (int) _children->size() - 1;
    while (left <= right) {
        // Avoid possible overflow of integers
        int mid = left + ((right-left) / 2);

        if (_children->at((size_t) mid)._word == word) {
            *index = mid;
            return true;
        }

        // After the left and right index are equal *and* the word hasn't been found there,
        // we can calculate where it should be insreted instead
        if (left == right) {    //mid._word != word here, so we return where the new word should be added.
            if (_children->at((size_t) left)._word > word) {
                *index = left;
            }
            else {
                *index = left + 1;
            }
            return false;
        }

        // Go to the left/right half accordingly
        if (_children->at((size_t) mid)._word > word) {
            right = mid - 1;
        }
        else if (_children->at((size_t) mid)._word < word) {
            left = mid + 1;
        }

    }
}

// Not used. Consider removing this
void trie::trie_node::push_child(trie_node *node) {
    if (_children == nullptr) {
        _children = new mstd::vector<trie_node>(SIZE);
    }
    _children->m_push(*node);
}

bool trie::trie_node::is_end_of_word() const {
    return _eow;
}

// Perhaps asteris will use this?
size_t trie::trie_node::children_size() const {
    return _children->size();
}

std::string trie::trie_node::get_word() const {
    return _word;
}

void trie::trie_node::set_end_of_word(bool v) {
    _eow = v;
}

// Can't be deleted because it's required by the templated vector (still not used)
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
