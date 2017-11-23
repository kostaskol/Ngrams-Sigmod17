#include <hash_table.hpp>
#include "trie.hpp"
#include "constants.hpp"
#include "bloom_filter.hpp"

using mstd::vector;
using std::string;
using std::cout;
using std::endl;
using mstd::logger;

extern bool __debug__;

/*
 * Trie Implementation
 */
trie::trie() : _num_nodes(0), _num_ngrams(0) {
    _root = new root_node();
}

trie::~trie() {
    delete _root;
}

void trie::add(const vector<string> &ngram) {
    // Start at the root
    // Insert (if required) and get the child
    trie_node *current = _root->add_child(ngram[0], false);

    // Go up until the previous to last part (we need to treat the last part differently)
    for (int i = 1; i < ngram.size() - 1; i++) {
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
    bloom_filter bf(BLOOM_SIZE, BLOOM_K);
    mstd::hash_table<void *> ht;
    std::stringstream ss , final_ss;
    bool found_one = false;
    bool one_word = false;

    for (size_t i = 0; i < ngram.size(); i++) {
        trie_node *current = _root->get_child(ngram[i]);
        trie_node *child;
        for (size_t j = i; j < ngram.size(); j++) {
            if ((child = current->get_child(ngram.at(j),nullptr)) == nullptr) {
                ss.str("");
                ss.clear();
                one_word = false;
                // current = _root; Current will be re-declared upon next iteration
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
                    if (__debug__) {
                        if (!bf.check(ss.str())) {
                            bf.insert(ss.str());
                            if (found_one) 
                                final_ss << "|";
                            
                            found_one = true;
                            final_ss << ss.str();
                        }
                    } else {
                        try {
                            // If the key cannot be found in the hash table, it throws an exception
                            // if it doesn't throw, it means that the key already exists (so we have already found the ngram)
                            // so we start from the beginning
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

bool trie::delete_ngram(const mstd::vector<std::string> &ngram) {
    trie_node *current/* = _root*/; // TODO: Factor in the trie node change
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
    trie_node *curr /* = _root*/; // TODO: Factor in the root change
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
    // Figure out how to print the tree (required by the unit tests
    //_root->print(0);
}

std::string trie::to_string() {
    std::stringstream ss;
    //_root->to_string(ss, 0);
    return ss.str();
}
