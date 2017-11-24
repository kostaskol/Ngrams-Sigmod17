#include <hash_table.hpp>
#include "trie.hpp"

using mstd::vector;
using std::string;
using std::cout;
using std::endl;
using mstd::logger;
using mstd::stack;

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
    bool one_word = false;

    if (_root->get_children_p() == nullptr) {
        results->push("$$EMPTY$$");
        return;
    }
    for (size_t i = 0; i < ngram.size(); i++) {
        trie_node *child;
        for (size_t j = i; j < ngram.size(); j++) {
            if ((child = current->get_child(ngram.at(j),nullptr)) == nullptr) {
                ss.str("");
                ss.clear();
                one_word = false;
                current = _root;
                break;
            }
            else{
                if (one_word) {
                    ss << " " + ngram.at(j);
                }
                else{
                    ss << ngram.at(j);
                    one_word = true;
                }
                if (child->is_end_of_word()) {
                    try {
                        // If the key cannot be found in the hash table, it throws an exception
                        // if it doesn't throw, it means that the key already exists (so we have already found the ngram)
                        ht.get(ss.str());
                    } catch (std::runtime_error &e) {
                        if (found_one) {
                            final_ss << "|";
                        }
                        // If it throws, the key doesn't exist, so we add it to the hash table and continue
                        ht.put(ss.str(), nullptr);
                        found_one = true;
                        final_ss << ss.str();
                    }
                }
                current = child;
            }
        }
    }
    if (!found_one) {
        results->push("$$END$$");
    }
    else {
        results->push(final_ss.str());
    }
}

void trie::compress() { return; }

bool trie::delete_ngram(const mstd::vector<std::string> &ngram) {
    trie_node *current = _root;
    auto *ch_indexes = new int[(int)ngram.size()];
    auto **parents = new trie_node *[(int)ngram.size()];

    for (int i = 0; i < ngram.size(); i++) {
        parents[i] = current;
        if ((current = current->get_child(ngram.get((size_t) i), &ch_indexes[i])) == nullptr) {
            // The requested N-gram does not exist on the Trie.
            delete[] ch_indexes;
            delete[] parents;
            return false;
        }
    }
    // Now the current point to the end_of_word node of the N-gram.
    if (current->is_end_of_word()) {
        if (current->get_children_p() == nullptr) {
            // The end_of_word node can be deleted, so its parent will delete him.
            parents[(int)ngram.size()-1]->remove_child(ch_indexes[(int)ngram.size()-1]);
        }
        else {
            // The end_of_word node cannot be deleted, so we un-check the end_of_word flag.
            current->set_end_of_word(false);
            delete[] ch_indexes;
            delete[] parents;
            return true;
        }
    }
    else {
        // The request N-gram does not exist on the Trie.
        delete[] ch_indexes;
        delete[] parents;
        return false;
    }
    current = parents[(int)ngram.size()-1];
    for (int i = (int)ngram.size()-2; i >= 0; i--) {
        if (current->is_end_of_word() || current->get_children_p() != nullptr) {
            // I am an end_of_word node for another N-gram of the Trie, so I cannot be deleted.
            // OR
            // I have more children after my child's deletion, so I cannot be deleted.
            delete[] ch_indexes;
            delete[] parents;
            return true;
        }
        else {
            // I do not satisfy any of the above conditions, so I can be deleted.
            parents[i]->remove_child(ch_indexes[i]);
            current = parents[i];
        }
    }

    delete[] ch_indexes;
    delete[] parents;
    return true;
}

// TODO: Keep this?
bool trie::r_delete_ngram(const mstd::vector<std::string> &ngram) {
    trie_node *curr = _root;
    int found;

    if (!r_delete_helper(ngram, curr, (int)ngram.size(), 0, &found)){
        if (!found){
            // Didn't find the requested N-gram.
            return false;
        }
    }
    // Successfully deleted the requested N-gram.
    return true;
}

bool trie::r_delete_helper(const mstd::vector<std::string> &ngram, trie_node *current, int length, int level, int *found) {
    trie_node *child;
    int ch_index;

    if (length != level) {
        // We still need to descend to the last node of the ngram.
        if ((child = current->get_child(ngram.get((size_t) level), &ch_index)) == nullptr) {
            // The requested N-gram does not exist on the Trie.
            *found = 0;
            return false;
        }
        if (r_delete_helper(ngram, child, length, level+1, found)) {
            // If my child marked itself for deletion I have to remove it from my children.
            current->remove_child(ch_index);
            // Check if I can mark myself for deletion as well.
            if (current->is_end_of_word()){
                // I am an end_of_word node for another N-gram of the Trie, so I cannot be deleted.
                return false;
            }
            else return current->get_children_p() == nullptr;
        }
        else {
            // If my child couldn't delete itself
            return false;
        }
    }
    else {
        // Base case of the algorithm. We are the end_of_word node.
        *found = 1;
        if (current->get_children_p() == nullptr) {
            // The end_of_word node can be deleted, so it tells its parent to delete him.
            return true;
        }
        else {
            // The end_of_word node cannot be deleted, so we un-check the end_of_word flag.
            current->set_end_of_word(false);
            return false;
        }
    }

}

void trie::print_tree() {
    _root->print(0);
}

std::string trie::to_string() {
    std::stringstream ss;
    _root->to_string(ss, 0);
    return ss.str();
}

/*
 * trie_node implementation
 */

trie::trie_node::trie_node()
        : _word(""), _eow(false) {
    _children = nullptr;
}

trie::trie_node::trie_node(const std::string &word, bool eow)
        : _word(word), _eow(eow) {
    _children = nullptr;
}

// Deleted copy constructor here since it isn't used anywhere (just as it should be)
// trie::trie_node::trie_node(const trie::trie_node &other)=delete;

trie::trie_node::trie_node(trie_node &&other) noexcept {
    _word = other._word;
    _eow = other._eow;
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
        // Better to fail fast than fail safe
         throw std::runtime_error("Index below zero");
        // If we want a fail safe method (worse imo), uncomment the following 2 lines
        // logger::warn("trie::trie_node::add_child", "Index below zero(" + std::to_string(index) + ")");
        // index = 0;
    }

    if (index > _children->size()) {
        // Better fail fast than fail safe
        throw std::runtime_error("Index above size (Index: " + std::to_string(index)
                                                   + " & Size: " + std::to_string(_children->size()) + ")");
        // If we want a fail safe method (worse imo), uncomment the following 3 lines
        // logger::warn("trie::trie_node::add_child", "Index above size (Index: " + std::to_string(index)
        //                                              + " & Size: " + std::to_string(_children->size()) + ")");
        // index = (int) _children->size();
    }

    trie_node new_node(word, eow);
    return _children->m_insert_at(index, new_node);
}

void trie::trie_node::remove_child(int index) {
    _children->remove_at((size_t) index);
    if (_children->size() == 0){
        delete _children;
        _children = nullptr;
    }
}

mstd::vector<trie::trie_node> *trie::trie_node::get_children_p() {
    return  _children;
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
        *index = (int)_children->size() - 1;
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
            if (_children->at((size_t) left)._word > word ) {
                *index = left;
            }
            else if (_children->at((size_t) right)._word < word) {
                *index = right + 1;
            }
            else if (_children->at((size_t) right)._word > word) {
                *index = right;
            }
            else {
                if (_children->at((size_t) right)._word == word) {
                    *index = right;
                } else {
                    *index = mid;
                }
                return true;
            }
            return false;
        }
        if (_children->at((size_t) mid)._word > word) {
            right = mid - 1;
        }
        else if (_children->at((size_t) mid)._word < word) {
            left = mid + 1;
        }
    }
}

bool trie::trie_node::is_end_of_word() const {
    return _eow;
}

std::string& trie::trie_node::get_word() {
    return _word;
}

void trie::trie_node::set_word(std::string word) {
    _word = word;
}

void trie::trie_node::add_short(std::string &word, bool eow) { return; }

void trie::trie_node::set_end_of_word(bool v) {
    _eow = v;
}

void trie::trie_node::set_children(mstd::vector<trie::trie_node> *c) {
    _children = c;
}

size_t trie::trie_node::get_children_size() {
    if (_children == nullptr){
        return 0;
    } else {
        return _children->size();
    }

}

void trie::trie_node::push_children(mstd::stack<trie::trie_node *> *s) {
    if (_children == nullptr) return;

    for (int i = (int)_children->size()-1; i >=0; i--) {
        s->push(_children->get_p((size_t)i));
    }
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

void trie::trie_node::to_string(std::stringstream &ss, int level) {
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

trie::trie_node &trie::trie_node::operator=(const trie::trie_node &other) {
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
trie::trie_node &trie::trie_node::operator=(trie::trie_node &&other) noexcept {
    _word = other._word;
    _eow = other._eow;
    _children = other._children;
    other._children = nullptr;
    return *this;
}

std::ostream &operator<<(std::ostream &out, const trie::trie_node &other) {
    out << other._word;
    return out;
}

/*
 * static_trie implementation
 */

static_trie::static_trie() : trie::trie() {
    _root = new static_node();
}

static_trie::~static_trie(){
    delete _root;
}

void static_trie::add(const mstd::vector<std::string> &ngram) {
    // Start at the root
    static_node *current = _root;

    // Go up until the previous to last part (we need to treat the last part differently)
    for (int i = 0; i < ngram.size() - 1; i++) {
        static_node *child;
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
    static_node *child;
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

void static_trie::compress() {
    static_node *current, *child;
    stack<trie_node *> s;
    std::stringstream ss;

    if (_root->get_children_p() == nullptr) return; //Empty static trie

//    int old_nodes = (int)_num_nodes;
    _num_nodes = 0;


    _root->push_children(&s);

    while (!s.empty()) {
        current = (static_node *)s.pop();
        if (current->get_children_p() == nullptr) {
            _num_nodes++;
            continue;
        }
        ss << current->get_word();
        while (current->get_children_size() == 1) {
            child = current->get_child(0);
            ss << " "+child->get_word();
            current->add_short(current->get_word(), current->is_end_of_word());
            current->add_short(child->get_word(), child->is_end_of_word());
            current->set_children(child->get_children_p());
            child->set_children(nullptr);
        }
        current->set_word(ss.str());
        ss.str("");
        ss.clear();
        _num_nodes++;
        current->push_children(&s);
    }
    cout << _num_nodes << endl;
}

void static_trie::print_tree() {
    _root->print(0);
}

std::string static_trie::to_string() {
    std::stringstream ss;
    _root->to_string(ss, 0);
    return ss.str();
}

/*
 * static_node implementation
 */

trie::static_node::static_node() : trie::trie_node::trie_node() {
    _lenofwords = nullptr;
}

trie::static_node::static_node(const std::string &word, bool eow) : trie::trie_node::trie_node(word, eow) {
    _lenofwords = nullptr;
}

trie::static_node::~static_node(){
    delete _lenofwords;
}

trie::static_node *trie::static_node::add_child(int index, std::string word, bool eow) {
    if (_children == nullptr) {
        _children = new mstd::vector<trie::trie_node>(SIZE);
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

trie::static_node *trie::static_node::get_child(int index) {
    return (static_node *)_children->at_p((size_t) index);
}

trie::static_node *trie::static_node::get_child(std::string &word, int *at) {
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

mstd::vector<signed short> *trie::static_node::get_lenofwords_p() {
    return _lenofwords;
}

void trie::static_node::add_short(std::string &word, bool eow) {
    if (_lenofwords == nullptr) {
        _lenofwords = new mstd::vector<signed short>(SIZE);
    }

    if (eow) {
        _lenofwords->push((signed short)word.length());
    } else {
        _lenofwords->push((signed short)(-word.length()));
    }

}

trie::static_node &trie::static_node::operator=(const trie::static_node &other) {
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
trie::static_node &trie::static_node::operator=(trie::static_node &&other) noexcept {
    _word = other._word;
    _eow = other._eow;
    _children = other._children;
    _lenofwords = other._lenofwords;
    other._children = nullptr;
    other._lenofwords = nullptr;
    return *this;
}