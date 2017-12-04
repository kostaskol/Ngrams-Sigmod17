#include <hash_table.hpp>
#include <binary_search.hpp>
#include "trie.hpp"
#include "bloom_filter.hpp"

using mstd::vector;
using std::string;
using std::cout;
using std::endl;
using mstd::logger;
using mstd::stack;

extern bool __debug__;

/*
 * Trie Implementation
 */
trie::trie() : _num_ngrams(0), _num_nodes(0) {
    _root = new root_node();
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
            current = current->add_child(ngram.at(i), false, index);
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
        current->add_child(last_word, true, index);
        _num_nodes++;
    }
    _num_ngrams++;
}

void trie::search(const vector<string> &ngram, mstd::queue<std::string> *results) {
    trie_node *current = _root;
    std::stringstream ss , final_ss;
    bloom_filter bf(constants::BLOOM_SIZE, constants::BLOOM_K);
    bool found_one = false;
    bool one_word = false;
    mstd::hash_table<void *> ht;

    if (_root->empty()) {
        results->push("$$EMPTY$$");
        return;
    }

    for (size_t i = 0; i < ngram.size(); i++) {
        trie_node *child;
        for (size_t j = i; j < ngram.size(); j++) {
            if ((child = current->get_child(ngram.at(j),nullptr)) == nullptr) {
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
                    if (!bf.check_and_set(ss.str())) {
                        // Ngram did not exist
                        if (found_one) {
                            final_ss << "|";
                        }
                        found_one = true;
                        final_ss << ss.str();
                    }
                }
                current = child;
            }
        }
        ss.str("");
        ss.clear();
        one_word = false;
        current = _root;
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
    trie_node *current = _root; // TODO: Factor in the trie node change
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

        if (!current->has_children()) {
            // The end_of_word node can be deleted, so its parent will delete him.
            if (parents[(int) ngram.size() - 1]->is_root()) {
                ((root_node*) parents[(int) ngram.size() - 1])->remove_child(current->get_word());
            } else {
                parents[(int)ngram.size()-1]->remove_child(ch_indexes[(int)ngram.size()-1]);
            }
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
        if (current->is_end_of_word() || current->has_children()) {
            // I am an end_of_word node for another N-gram of the Trie, so I cannot be deleted.
            // OR
            // I have more children after my child's deletion, so I cannot be deleted.
            delete[] ch_indexes;
            delete[] parents;
            return true;
        }
        else {
            // I do not satisfy any of the above conditions, so I can be deleted.
            if (parents[i]->is_root()) {
                // It's parent is the root node, so we need to supply the string rather than the index
                ((root_node *) parents[i])->remove_child(current->get_word());
            } else {
                parents[i]->remove_child(ch_indexes[i]);
            }
            current = parents[i];
        }
    }

    delete[] ch_indexes;
    delete[] parents;
    return true;
}

std::string trie::to_string() {
    std::stringstream ss;
    _root->to_string(ss, 0);
    return ss.str();
}

/*
 * static_trie implementation
 */

static_trie::static_trie() : trie::trie() {
    _root = new static_root_node();
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
            current = current->add_child(ngram.at(i), false, index);
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
        current->add_child(last_word, true, index);
        _num_nodes++;
    }

    _num_ngrams++;
}

void static_trie::search(const vector<string> &ngram, mstd::queue<std::string> *results) {
    static_node *current = _root;
    std::stringstream ss , final_ss;
    bloom_filter bf(constants::BLOOM_SIZE, constants::BLOOM_K);
    bool found_one = false;
    bool one_word = false;
    mstd::hash_table<void *> ht;

    if (_root->empty()) {
        results->push("$$EMPTY$$");
        return;
    }

    for (size_t i = 0; i < ngram.size(); i++) {
        static_node *child = nullptr;
        int k = 0;
        for (size_t j = i; j < ngram.size(); j++) {
            if (!k) {
                if ((child = current->search_static_child(ngram[j])) == nullptr) {
                    break;
                } else {
                    if (one_word) {
                        ss << " " + ngram.at(j);
                    }
                    else{
                        ss << ngram.at(j);
                        one_word = true;
                    }
                    if (child->end_of_word(k)) {
                        if (!bf.check_and_set(ss.str())) {
                            // Ngram did not exist
                            if (found_one) {
                                final_ss << "|";
                            }
                            found_one = true;
                            final_ss << ss.str();
                        }
                    }
                    if (child->lenofwords_size() > 1){
                        k++;
                    } else {
                        k = 0;
                        current = child;
                    }
                }
            } else {
                //Traversing in the same node.
                if (k >= child->lenofwords_size()) {
                    k = 0;
                    current = child;
                } else {
                    if (child->get_word(k) == ngram.at(j)) {
                        if (one_word) {
                            ss << " " + ngram.at(j);
                        } else {
                            ss << ngram.at(j);
                            one_word = true;
                        }

                        if (child->end_of_word(k)) {
                            if (!bf.check_and_set(ss.str())) {
                                // Ngram did not exist
                                if (found_one) {
                                    final_ss << "|";
                                }
                                found_one = true;
                                final_ss << ss.str();
                            }
                        }
                        k++;
                        if (k >= child->lenofwords_size()) {
                            k = 0;
                            current = child;
                        }
                    } else {
                        break;
                    }
                }
            }
        }
        ss.str("");
        ss.clear();
        one_word = false;
        current = _root;
    }
    if (!found_one) {
        results->push("$$END$$");
    }
    else {
        results->push(final_ss.str());
    }
}

void static_trie::compress() {
    static_node *current, *child;
    stack<static_node *> s;
    std::stringstream ss;

    if (_root->empty()) return; //Empty static trie

    _num_nodes = 0;

    _root->push_children(&s);
    while (!s.empty()) {
        current = s.pop();
        if (current->get_st_children_p() == nullptr) {
            current->add_short(((trie_node*)current)->get_word(), current->is_end_of_word());
//            current->print_shorts();
            _num_nodes++;
            continue;
        }
        ss << ((trie_node*)current)->get_word();
        current->add_short(((trie_node*)current)->get_word(), current->is_end_of_word());

        while (current->get_children_size() == 1) {
            child = current->get_child(0);
            ss << ((trie_node*)child)->get_word();
            current->add_short(((trie_node*)child)->get_word(), child->is_end_of_word());
            current->steal_children(child->get_st_children_p());
        }
        current->set_word(ss.str());
        ss.str("");
        ss.clear();
//        current->print_shorts();
        _num_nodes++;
        current->push_children(&s);
    }
}

void trie::print_tree() {}

void static_trie::print_tree() {
    _root->print();
}

std::string static_trie::to_string() {
    std::stringstream ss;
    _root->to_string(ss, 0);
    return ss.str();
}