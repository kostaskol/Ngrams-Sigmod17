#include "trie.hpp"
#include "logger.hpp"
#include "helpers.hpp"

using mstd::vector;
using std::string;
using std::cout;
using std::endl;

#define SIZE 3

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

void trie::add(const vector<string> &ngram) {
    cout << "NEW NGRAM: " << helpers::join(ngram, ' ') << "\n" << endl;
    // Start at the root
    trie_node *current = _root;

    // Go up until the previous to last part (we need to treat the last part differently)
    for (int i = 0; i < ngram.size() - 1; i++) {

        trie_node *child;
        if ((child = current->get_child(ngram.at(i))) == nullptr) {
            // If the current trie_node doesn't already contain that child, add it (not as an end of word)
//             std::cout << "Prosthesa to paidi : " << current->get_word() << '\n';
//            cout << "Inserted " << ngram.at(i) << endl;
            current = current->add_child(ngram.at(i), false);
//            cout << "And got back " << current->get_word() << endl;
        } else {
            // Otherwise, follow that child's path
            current = child;
        }
    }

    std::string last_word = ngram.get_cpy((int) ngram.size() - 1);
    trie_node *child;
    if ((child = current->get_child(last_word)) != nullptr) {
        // If the word already existed in the tree, we simply mark it as the end of the N-Gram
//        cout << "Setting " << child->get_word() << " as end of word" << endl;
        child->set_end_of_word();
    } else {
        // Otherwise we insert it to that node's children
//        cout << "Inserted " << last_word << endl;
        current = current->add_child(last_word, true);
//        cout << "And got back " << current->get_word() << endl;
    }
    _size++;
}


// To be deleted
bool trie::search(const vector<string> &ngram) {
    trie_node *current = _root;
    for (int i = 0; i < ngram.size() - 1; i++) {
        if ((current = current->get_child(ngram.get(i))) == nullptr) {
            return false;
        }
    }

    return ((current = current->get_child(ngram.get((int) ngram.size() - 1))) != nullptr && current->is_end_of_word());
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
    if (_children == nullptr) {
        _children = new mstd::vector<trie_node>(SIZE);
    }

    size_t pos = 0;
    trie_node new_node(word, eow, this);
    if (_children->size() == 0) {
        return _children->m_push(new_node);
        // The position of the child is the position of the last element of the vector
    } else {
        // ΝΟΤΕ: Only for testing purposes

        for (int i = 0; i < _children->size(); i++) {
            if (_children->get((size_t) i)._word < word) {
                return _children->m_insert_at((size_t) i, new_node);
            }
        }
        // We know that the child does not exist, but no suitable place was found for it.
        // That means that it's larger than the last element, which means we can simply push
        return _children->m_push(new_node);
        // TODO: Replace for loop with bsearch
//        int at;
//        if (!_bsearch_children(word, &at)) {
//K//            If the bsearch "returns" -1, that means that we can simply push the child into the vector
//            if (at == -1) {
//                return _children->m_push(new_node);
//            }
//            return _children->m_insert_at((size_t) at, new_node);
//        } else{
//            return nullptr; //if we are careful to call add_child only when the child we are adding doesnt not already exists, this line is not useful
//        }
    }

    return &_children->get(pos);
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
    } else{
        return get_child(index);
    }
}

/*
 Ένα απλό implementation που λύνει τα corner cases
 bool bsearch(const vector<string> &v, string w, int *index) {
    int left = 0;
    int right = (int) v.size() - 1;
    while (left <= right) {
        int mid = (right + left) >> 1;
        if (v[mid] == w) {
            *index = mid;
            return true;
        }

        if (left == right) {
            if (v[left] == w) {
                *index = left;
                return true;
            } else if (v[left] > w) {
                *index = left;
                return false;
            } else {
                *index = left + 1;
                return false;
            }
        }

        if (v[mid] < w) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
}
 */
bool trie::trie_node::_bsearch_children(std::string &word, int *index) {
    // FIXME(giannis): Η found δεν χρειάζεται. Δεν γίνεται ποτέ true χωρίς να επιστραφεί αμέσως
    // TODO(giannis): Θα μπορούσε να χρησιμοποιηθεί αυτό για να γλυτώσουμε υπολογισμούς
//    if (_children->at(0)._word > word) {
//        *index = 0;
//        return false;
//    }
//    if (_children->at(_children->size() - 1)._word < word) {
////      Αν επιστραφεί -1, θα σημαίνει πως μπορούμε απλά να κάνουμε push για να μην πέσουμε πάνω σε κάποιο corner case
//        *index = -1;
//        return false;
//    }
    bool found = false;
    int left = 0;
    int right = (int) _children->size() - 1;
    while (left <= right) {
        // FIXME(giannis): Not required. It's the same as (left + right) >> 1
        // left + (right - left) / 2 = left + right / 2 - left / 2 = left - left / 2 + right / 2 = left / 2 + right / 2 = (left + right) / 2
        int mid = left + ((right-left) / 2);
        if (_children->at(mid)._word == word) {
            found = true;
            *index = mid;
            return found;
        }

        // FIXME(giannis): Δεν χρειάζεται. Το ίδιο με: _children->at(left)._word > word
        if (left == right) {    //mid._word != word here, so we return where the new word should be added.
            // Note: *Νομίζω* πως εδώ πρέπει να ελέγχει και αν είναι ίσα γιατί μπορεί όταν φτάσει στο τελευταίο
            // split να βρεθεί το παιδί (ίσως να κάνω λάθος. Πρέπει να ελεγχθεί περισσότερο)
            if (_children->at(left)._word.compare(word) > 0) {
                // FIXME(giannis): Το index θα έπρεπε να είναι left
                *index = left+1;
            }
            else{
                // FIXME(giannis): *Νομίζω* πως το index θα έπρεπε να είναι left + 1
                *index = left;
            }
            return found;
        }

        // FIXME(giannis): Αν το word είναι μεγαλύτερο από το _children[mid]._word, τότε πρέπει να πας στο δεξί μισό (άρα left = mid + 1),
        // αλλιώς right = mid - 1;
        /// Η _word.compare(word) επιστρέφει αρνητικό αν το *_word* είναι μικρότερο του *word* και όχι ανάποδα
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
        _children = new mstd::vector<trie_node>(SIZE);
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
