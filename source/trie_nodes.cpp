#include "trie_nodes.hpp"
#include "mvector.hpp"
#include "binary_search.hpp"
#include "logger.hpp"
#include <string>

using mstd::vector;
using mstd::logger;
using std::string;
using std::cout; using std::endl;


/*
 * trie node implementation
 */
trie_node::trie_node()
        : _word(""), _eow(false) {
    _children = nullptr;
}

trie_node::trie_node(const std::string &word, bool eow)
        : _word(word), _eow(eow) {
    _children = nullptr;
}

// Deleted copy constructor here since it isn't used anywhere (just as it should be)
// trie::trie_node::trie_node(const trie::trie_node &other)=delete;

trie_node::trie_node(trie_node &&other) noexcept {
    _word = other._word;
    _eow = other._eow;
    _children = other._children;
    other._children = nullptr;
}

trie_node::~trie_node() {
    delete _children;
}

trie_node *trie_node::add_child(std::string &word, bool eow, int index) {
    if (_children == nullptr) {
        _children = new mstd::vector<trie_node>(CHILDREN_INITIAL_SIZE /* 3 */);
    }

    if (index < 0) {
        // Better to fail fast than fail safe
        throw std::runtime_error("Index below zero");
        // If we want a fail safe method (worse imo), uncomment the following 2 lines
        // logger::warn("trie_node::add_child", "Index below zero(" + std::to_string(index) + ")");
        // index = 0;
    }

    if (index > _children->size()) {
        // Better fail fast than fail safe
        throw std::runtime_error("Index above size (Index: " + std::to_string(index)
                                 + " & Size: " + std::to_string(_children->size()) + ")");
        // If we want a fail safe method (worse imo), uncomment the following 3 lines
        // logger::warn("trie_node::add_child", "Index above size (Index: " + std::to_string(index)
        //                                              + " & Size: " + std::to_string(_children->size()) + ")");
        // index = (int) _children->size();
    }

    trie_node new_node(word, eow);
    return _children->m_insert_at(index, new_node);
}

const std::string &trie_node::get_word() { return _word; }

void trie_node::set_word(const std::string &word) {
    _word = word;
}

void trie_node::remove_child(int index) {
    _children->remove_at((size_t) index);
    if (_children->size() == 0){
        delete _children;
        _children = nullptr;
    }
}

vector<trie_node> *trie_node::get_children_p() {
    return  _children;
}

trie_node *trie_node::get_child(int index) {
    return _children->at_p((size_t) index);
}

trie_node *trie_node::get_child(const std::string &word, int *at) {
    // NOTE: Potential bug caught here.
    // Avoided simply because the variable passed by add was initialised to 0
    // at was not properly set if the _children vector was null
    if (_children == nullptr) {
        if (at != nullptr) {
            *at = 0;
        }
        return nullptr;
    }
    int index;
    if(!bsearch_children(word, *_children, &index)) {    // Not found
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

bool trie_node::is_root() const {
    return false;
}



bool trie_node::is_end_of_word() const {
    return _eow;
}

void trie_node::add_short(const std::string &word, bool eow) { return; }

void trie_node::set_end_of_word(bool v) {
    _eow = v;
}

void trie_node::steal_children(mstd::vector<trie_node> *c) {
    mstd::vector<trie_node> *temp;
    if (c != nullptr) {
        temp = new mstd::vector<trie_node>(std::move(*c));
    } else {
        temp = nullptr;
    }

    delete _children;
    _children = temp;
}

size_t trie_node::get_children_size() {
    if (_children == nullptr){
        return 0;
    } else {
        return _children->size();
    }

}

void trie_node::push_children(mstd::stack<trie_node *> *s) {
    if (_children == nullptr) return;

    for (int i = (int)_children->size()-1; i >=0; i--) {
        s->push(_children->get_p((size_t)i));
    }
}

void trie_node::print(int level) {
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

void trie_node::to_string(std::stringstream &ss, int level) {
    for (int i = 1; i < level; i++) {
        ss << "\t";
    }
    if (_word != "") {
        ss << _word << "\n";
    }
    if (_children == nullptr) return;
    for (size_t i = 0; i < _children->size(); i++) {
        _children->get(i).to_string(ss, level + 1);
    }
}

bool trie_node::has_children() const {
    return _children != nullptr;
}

trie_node &trie_node::operator=(const trie_node &other) {
    _word = other._word;
    _eow = other._eow;
    if (other._children != nullptr) {
        delete _children;
        _children = new mstd::vector<trie_node>(*other._children);
    }
    return *this;
}

// Move assignment operator
// "Steals" the _children pointer from the other
// trie_node
trie_node &trie_node::operator=(trie_node &&other) noexcept {
    _word = other._word;
    _eow = other._eow;
    _children = other._children;
    other._children = nullptr;
    return *this;
}

std::ostream &operator<<(std::ostream &out, const trie_node &other) {
    out << other._word;
    return out;
}

/*
 * static_node implementation
 */

static_node::static_node() : trie_node::trie_node() {
    _lenofwords = nullptr;
}

static_node::static_node(const std::string &word, bool eow) : trie_node::trie_node(word, eow) {
    _lenofwords = nullptr;
}

static_node::~static_node(){
    delete _lenofwords;
}

static_node *static_node::add_child(std::string &word, bool eow, int index) {
    if (_children == nullptr) {
        _children = new mstd::vector<trie_node>(CHILDREN_INITIAL_SIZE /* 3 */);
    }

    if (index < 0) {
        // Better to fail fast than fail safe
        throw std::runtime_error("Index below zero");
    }

    if (index > _children->size()) {
        // Better fail fast than fail safe
        throw std::runtime_error("Index above size (Index: " + std::to_string(index)
                                 + " & Size: " + std::to_string(_children->size()) + ")");
    }

    static_node new_node(word, eow);
    return (static_node *)_children->m_insert_at(index, new_node);
}

static_node *static_node::get_child(int index) {
    return (static_node *)_children->at_p((size_t) index);
}

static_node *static_node::get_child(const std::string &word, int *at) {
    if (_children == nullptr) {
        if (at != nullptr) {
            *at = 0;
        }
        return nullptr;
    }
    int index;
    if(!bsearch_children(word, *_children, &index)) {    // Not found
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

mstd::vector<signed short> *static_node::get_lenofwords_p() {
    return _lenofwords;
}

void static_node::add_short(const std::string &word, bool eow) {
    if (_lenofwords == nullptr) {
        _lenofwords = new mstd::vector<signed short>(CHILDREN_INITIAL_SIZE /* 3 */);
    }

    if (eow) {
        _lenofwords->push((signed short)word.length());
    } else {
        _lenofwords->push((signed short)(-word.length()));
    }

}

static_node &static_node::operator=(const static_node &other) {
    _word = other._word;
    _eow = other._eow;
    if (other._children != nullptr) {
        delete _children;
        _children = new mstd::vector<trie_node>(*other._children);
    }
    if (other._lenofwords != nullptr) {
        delete _lenofwords;
        _lenofwords = new mstd::vector<signed short>(*other._lenofwords);
    }
    return *this;
}

// Move assignment operator
// "Steals" the _children pointer and the _lenofwords pointer
// from the other static_node
static_node &static_node::operator=(static_node &&other) noexcept {
    _word = other._word;
    _eow = other._eow;
    _children = other._children;
    _lenofwords = other._lenofwords;
    other._children = nullptr;
    other._lenofwords = nullptr;
    return *this;
}

/*
 * root node implementation
 */
root_node::root_node(size_t initial_size) : _children(initial_size) { }

trie_node *root_node::add_child(string &word, bool eow, /* Not actually used. Required for polymorphism > */ int index) {
    logger::debug("root_node::add_child", "Adding child: " + word, LOGFILE);
    return _children.insert(word, eow);
}

void root_node::remove_child(const string &word) {
    _children.delete_word(word);
}

trie_node *root_node::get_child(const std::string &word, int *at) {
    return _children.get(word);
}

bool root_node::is_root() const {
    return true;
}

bool root_node::has_children() const {
    return !_children.empty();
}

void root_node::print() {
    _children.print();
}

bool root_node::empty() {
    return _children.empty();
}