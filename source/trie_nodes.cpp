#include "trie_nodes.hpp"
#include "mvector.hpp"
#include "binary_search.hpp"
#include "logger.hpp"
#include <string>

using mstd::vector;
using mstd::logger;
using mstd::stack;
using std::string;
using std::cout; using std::endl;


/*
 * trie node implementation
 */
trie_node::trie_node()
        : _word(""),
        _eow(false),
        _ver_added(0),
        _ver_deleted(-1),
        _marked_for_del(false) {

    _children = nullptr;
}

trie_node::trie_node(const std::string &word, bool eow, int version)
        : _word(word),
        _eow(eow),
        _ver_added(version),
        _ver_deleted(-1),
        _marked_for_del(false) {

    _children = nullptr;
}

// Deleted copy constructor here since it isn't used anywhere (just as it should be)
// trie::trie_node::trie_node(const trie::trie_node &other)=delete;

trie_node::trie_node(trie_node &&other) noexcept {
    _word = other._word;
    _eow = other._eow;
    _children = other._children;
    _ver_added = other._ver_added;
    _ver_deleted = other._ver_deleted;
    _marked_for_del = other._marked_for_del;
    other._children = nullptr;
}

trie_node::~trie_node() {
    delete _children;
}

trie_node *trie_node::add_child(std::string &word, bool eow, int index, int version) {
//    cout << "DYNAMC ADD" << endl;
    if (_children == nullptr) {
        _children = new mstd::vector<trie_node>(constants::TRIE_CHILDREN_INIT_SIZE /* 3 */);
    }

    if (index < 0) {
        // Better to fail fast than fail safe
        throw std::runtime_error("Index below zero");
        // If we want a fail safe method (worse imo), uncomment the following 2 lines
        // logger::warn("trie_node::add_child", "Index below zero(" + std::to_string(index) + ")");
        // index = 0;
    }

    if (index > (int) _children->size()) {
        // Better fail fast than fail safe
        throw std::runtime_error("Index above size (Index: " + std::to_string(index)
                                 + " & Size: " + std::to_string(_children->size()) + ")");
        // If we want a fail safe method (worse imo), uncomment the following 3 lines
        // logger::warn("trie_node::add_child", "Index above size (Index: " + std::to_string(index)
        //                                              + " & Size: " + std::to_string(_children->size()) + ")");
        // index = (int) _children->size();
    }

    trie_node new_node(word, eow, version);
    return _children->m_insert_at(index, new_node);
}

const std::string &trie_node::get_word() const { return _word; }

void trie_node::set_child_del_version(int index, int version) {
    _children->get((size_t) index).set_del_version(version);
}

void trie_node::delete_child(int index) {
    if (_children == nullptr) {
        return;
    }
    _children->remove_at((size_t) index);
    if (_children->size() == 0){
        delete _children;
        _children = nullptr;
    }
}

void trie_node::delete_child(const string &word) {
    int index;
    if (get_child(word, &index) != nullptr) {
        delete_child(index);
    }
}

trie_node *trie_node::get_child(int index) {
    if (_children == nullptr || (size_t) index >= _children->size()) {
        return nullptr;
    }

    return _children->at_p((size_t) index);
}

trie_node *trie_node::get_child(const std::string &word, int *at) {
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

bool trie_node::is_end_of_word() const {
    return _eow;
}

void trie_node::set_end_of_word(bool v) {
    _eow = v;
}

void trie_node::set_add_version(int version) {
    _ver_added = version;
}

void trie_node::set_del_version(int version) {
    _ver_deleted = version;
}

int trie_node::get_add_version() {
    return _ver_added;
}

int trie_node::get_del_version() {
    return _ver_deleted;
}

void trie_node::set_mark_for_del(bool v) {
    _marked_for_del = v;
}

bool trie_node::is_marked_for_del() {
    return _marked_for_del;
}


void trie_node::delete_marked_children() {
    if (_children == nullptr) {
        return;
    }

    for (int i = (int) _children->size() - 1; i >= 0; i--) {
        if (_children->get((size_t) i).is_marked_for_del()) {
            this->delete_child(i);
        }
    }
}

size_t trie_node::get_children_size() const {
    return _children != nullptr ? _children->size() : 0;
}

void trie_node::push_children(stack<trie_node *> *s) {
    if (_children == nullptr) return;

    for (int i = 0; i < (int) _children->size(); i++) {
        s->push(_children->get_p((size_t) i));
    }
}

void trie_node::push_children(stack<tuple<trie_node *, int>> *s) {
    if (_children == nullptr) {
        return;
    }

    for (int i = 0; i < (int) _children->size(); i++) {
        tuple<trie_node *, int> t(_children->get_p((size_t) i), i);
        s->push(t);
    }
}

void trie_node::push_children(stack<vector<tuple<trie_node *, int>>> *s,
        vector<tuple<trie_node *, int>> &path) {
    if (_children == nullptr) {
        return;
    }

    for (int i = 0; i < (int) _children->size(); i++) {
        tuple<trie_node *, int> t(_children->get_p((size_t) i), i);
        vector<tuple<trie_node *, int>> tmp(path);
        tmp.push(t);
        s->push(tmp);
    }
}

trie_node &trie_node::operator=(const trie_node &other) {
    _word = other._word;
    _eow = other._eow;
    _ver_added = other._ver_added;
    _ver_deleted = other._ver_deleted;
    _marked_for_del = other._marked_for_del;
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
    _ver_added = other._ver_added;
    _ver_deleted = other._ver_deleted;
    _marked_for_del = other._marked_for_del;
    _children = other._children;
    other._children = nullptr;
    return *this;
}

/*
 * root node implementation
 */
root_node::root_node(size_t initial_size) : _children(initial_size) { }

trie_node *root_node::add_child(string &word, bool eow, int version) {
    trie_node new_node(word, eow, version);
    return _children.insert(new_node);
}

void root_node::set_child_del_version(const string &word, int version) {
    _children.get(word)->set_del_version(version);
}

void root_node::delete_child(const string &word) {
    _children.delete_word(word);
}

trie_node *root_node::get_child(const std::string &word) {
    return _children.get(word);
}

size_t root_node::get_children_size() const {
    return _children.get_num_items();
}

trie_node **root_node::get_top_branches(int *size) {
    return _children.get_top_branches(size);
}

void root_node::push_children(mstd::stack<trie_node *> *s) {
    _children.push_to_stack(s);
}

bool root_node::empty() {
    return _children.empty();
}


/*
 * static_node implementation
 */

static_node::static_node() : _word(""), _eow(false) {
    _children = nullptr;
    _lenofwords = nullptr;
}

static_node::static_node(const std::string &word, bool eow) : _word(word), _eow(eow) {
    _children = nullptr;
    _lenofwords = nullptr;
}

static_node::~static_node(){
    delete _children;
    delete _lenofwords;
}

static_node *static_node::add_child(std::string &word, bool eow, int index) {
    if (_children == nullptr) {
        _children = new mstd::vector<static_node>(constants::TRIE_CHILDREN_INIT_SIZE /* 3 */);
    }

    if (index < 0) {
        // Better to fail fast than fail safe
        throw std::runtime_error("Index below zero");
    }

    if (index > (int) _children->size()) {
        // Better fail fast than fail safe
        throw std::runtime_error("Index above size (Index: " + std::to_string(index)
                                 + " & Size: " + std::to_string(_children->size()) + ")");
    }

    static_node new_node(word, eow);
//    cout << "STATIC ADD" << endl;
    return _children->m_insert_at(index, new_node);
}

static_node *static_node::get_child(int index) {
    return _children->at_p((size_t) index);
}

static_node *static_node::get_child(const std::string &word, int *at) {
    if (_children == nullptr) {
        if (at != nullptr) {
            *at = 0;
        }
        return nullptr;
    }
    if (_lenofwords == nullptr) {
        //The static_trie is like a normal trie
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
    } else {
        //The static_trie is now compressed.
        return nullptr;
    }
}

static_node *static_node::search_static_child(const std::string &word) {
    if (_children == nullptr) {
//        logger::debug("static_node::search_static_child", "_children was null. Query: " + word);
        return nullptr;
    }

    return static_bsearch(word, *_children);
}

mstd::vector<static_node> *static_node::get_st_children_p(){
    return _children;
}

mstd::vector<signed short> *static_node::get_lenofwords_p() {
    return _lenofwords;
}

void static_node::steal_children(mstd::vector<static_node> *c) {
    mstd::vector<static_node> *temp;
    if (c != nullptr) {
        temp = new mstd::vector<static_node>(std::move(*c));
    } else {
        temp = nullptr;
    }

    delete _children;
    _children = temp;
}

void static_node::push_children(mstd::stack<static_node *> *s) {
    if (_children == nullptr) return;

    for (int i = (int)_children->size()-1; i >=0; i--) {
        s->push(_children->get_p((size_t)i));
    }
}

size_t static_node::get_children_size() {
    if (_children == nullptr){
        return 0;
    } else {
        return _children->size();
    }

}

bool static_node::end_of_word(int index) {
    return _lenofwords->at((size_t)index) > 0;
}

std::string static_node::get_word() {
    return _word;
}

std::string static_node::get_word(int index) {
//    logger::debug("static_node::get_word", "get_word(index) called");
    int sum = 0;
    for (int i = 0; i < index; i++) {
        sum += abs(_lenofwords->at((size_t)i));
    }
    return _word.substr((size_t) sum, (size_t)abs(_lenofwords->at((size_t) index)));
}

void static_node::set_word(string s) {
    _word = std::move(s);
}

size_t static_node::lenofwords_size() {
    if (_lenofwords == nullptr) return 0;
    return _lenofwords->size();
}

bool static_node::has_children() const {
    return _children != nullptr && _children->size() > 0;
}

void static_node::set_end_of_word(bool v) {
    _eow = v;
}

bool static_node::is_end_of_word() {
    return _eow;
}

void static_node::add_short(const std::string &word, bool eow) {
    if (_lenofwords == nullptr) {
        _lenofwords = new mstd::vector<signed short>(constants::TRIE_CHILDREN_INIT_SIZE /* 3 */);
    }

    if (eow) {
        _lenofwords->push((signed short)word.length());
    } else {
        _lenofwords->push((signed short)(-word.length()));
    }
}

void static_node::print_shorts() {
    cout << _word << "    ";
    for (int i = 0; i < (int) _lenofwords->size(); i++){
        cout << _lenofwords->get((size_t)i) << " ";
    }
    cout << endl;
}

static_node &static_node::operator=(const static_node &other) {
    _word = other._word;
    _eow = other._eow;
    if (other._children != nullptr) {
        delete _children;
        _children = new mstd::vector<static_node>(*other._children);
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
 * static_root node implementation
 */

static_root_node::static_root_node(size_t initial_size) : _children(initial_size) { }

static_node *static_root_node::add_child(std::string &word, bool eow) {
    static_node new_node(word, eow);
    return _children.insert(new_node);
}

static_node *static_root_node::get_child(const std::string &word) {
    return _children.get(word);
}

static_node *static_root_node::search_static_child(const std::string &word) {
    return _children.get_static(word);
}

bool static_root_node::has_children() const {
    return !_children.empty();
}

bool static_root_node::empty() {
    return _children.empty();
}

static_node **static_root_node::get_top_branches(int *size) {
    return _children.get_top_branches(size);
}

void static_root_node::push_children(mstd::stack<static_node *> *s) {
    _children.push_to_stack(s);
}
