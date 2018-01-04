#include <binary_search.hpp>
#include <sstream>
#include "trie.hpp"
#include "bloom_filter.hpp"

using mstd::vector;
using std::string;
using std::cout;
using std::endl;
using std::stringstream;
using mstd::logger;
using mstd::stack;
using mstd::queue;

/*
 * Trie Implementation
 */
trie::trie() {
    _root = new root_node();
}

trie::~trie() {
    delete _root;
}

void trie::add(const vector<string> &ngram, int version) {
    // Start at the root
    trie_node *current = _root;

    // Go up until the previous to last part (we need to treat the last part differently)
    for (int i = 0; i < (int) ngram.size() - 1; i++) {
        trie_node *child;
        int index;

        if (current == _root) {
            child = (reinterpret_cast<root_node *>(current))->get_child(ngram[i]);
        } else {
            child = current->get_child(ngram[i], &index);
        }
        
       if (child == nullptr) {
            // If the current trie_node doesn't already contain that child, add it (not as an end of word)
            if (current == _root) {
                current = (reinterpret_cast<root_node *>(current))->add_child(ngram[i], false, version);
            } else {
                current = current->add_child(ngram[i], false, index, version);
            }
        } else {
            // Even if the child was marked for deletion,
            // since it is now a path to a new ngram, it should not be deleted
            child->set_mark_for_del(false);

            current = child;
        }
    }
    

    std::string &last_word = ngram.get(ngram.size() - 1);
    trie_node *child;
    int index = 0;

    if (current == _root) {
        child = (reinterpret_cast<root_node *>(current))->get_child(last_word);
    } else {
        child = current->get_child(last_word, &index);
    }
    if (child != nullptr) {
        // THe word already existed in the trie
        child->set_end_of_word(true);
        child->set_add_version(version);
        child->set_mark_for_del(false);
    } else {
        // Otherwise we insert it to that node's children
        if (current == _root) {
            (reinterpret_cast<root_node *>(current))->add_child(last_word, true, version);
        } else {
            current->add_child(last_word, true, index, version);
        }
    }
}

string trie::search(const vector<string> &ngram, int version) {
    trie_node *current = _root;
    std::stringstream ss , final_ss;
    bloom_filter bf(constants::BLOOM_SIZE, constants::BLOOM_K);
    bool found_one = false;
    bool one_word = false;

    if (_root->empty()) {
        return "$$EMPTY$$";
    }

    for (size_t i = 0; i < ngram.size(); i++) {
        trie_node *child;
        for (size_t j = i; j < ngram.size(); j++) {
            if (current == _root) {
                child = (reinterpret_cast<root_node *>(current))->get_child(ngram[j]);
            } else {
                child = current->get_child(ngram[j], nullptr);
            }

            if (child == nullptr) {
                break;
            }
            else {
                if (one_word) {
                    ss << " " + ngram.at(j);
                }
                else {
                    ss << ngram.at(j);
                    one_word = true;
                }
               if (child->is_end_of_word()
                        && child->get_add_version() < version 
                        && (child->get_del_version() == -1
                            || child->get_del_version() > version)) {
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
        return "$$END$$";
    }
    else {
        return final_ss.str();
    }
}

bool trie::delete_ngram(const mstd::vector<std::string> &ngram, int version) {
    trie_node *current = _root; 
    auto *ch_indexes = new int[(int)ngram.size()];
    auto **parents = new trie_node *[(int)ngram.size()];

    for (int i = 0; i < (int) ngram.size(); i++) {
        parents[i] = current;
        if (current == _root) {
            current = (reinterpret_cast<root_node *>(current))->get_child(ngram[i]);
        } else {
            current = current->get_child(ngram[i], &ch_indexes[i]);
        }
        
        if (current == nullptr) {
            // The requested N-gram does not exist on the Trie.
            delete[] ch_indexes;
            delete[] parents;
            return false;
        }
    }
    // Now the current point to the end_of_word node of the N-gram.
    if (current->is_end_of_word()) {
        if (current->get_children_size() == 0) {
            // The end_of_word node can be deleted, so its parent will delete him.
            trie_node *tmp = parents[ngram.size() - 1];
            
            // While moving up the branch, we mark all nodes for deletion (and the last one that can
            // be deleted will actually be deleted)
            current->set_mark_for_del(true);

            if (tmp == _root) {
                (reinterpret_cast<root_node *>(tmp))->set_child_del_version(current->get_word(), version);
                        } else {
                tmp->set_child_del_version(ch_indexes[ngram.size() - 1], version);
            }
        }
        else {
            // The end_of_word node cannot be deleted, so we set its deletion version
            // current->set_end_of_word(false);
            current->set_del_version(version);
            delete[] ch_indexes;
            delete[] parents;
            return true;
        }
    }
    else {
        // The requested N-gram does not exist on the Trie.
        delete[] ch_indexes;
        delete[] parents;
        return false;
    }
    current = parents[(int)ngram.size()-1];
    for (int i = (int)ngram.size()-2; i >= 0; i--) {
        if (current != _root && (current->is_end_of_word() || current->get_children_size() != 0)) {
            // I am an end_of_word node for another N-gram of the Trie, so I cannot be deleted.
            // OR
            // I have more children after my child's deletion, so I cannot be deleted.
            delete[] ch_indexes;
            delete[] parents;
            return true;
        }
        else {
            // I do not satisfy any of the above conditions, so I can be deleted.
            trie_node *tmp = parents[i];

            // We mark ourself for deletion
            current->set_mark_for_del(true);
            if (tmp == _root) {
                (reinterpret_cast<root_node *>(tmp))->set_child_del_version(current->get_word(), version);
            } else {
                tmp->set_child_del_version(ch_indexes[i], version);
            }
            current = parents[i];
        }
    }

    delete[] ch_indexes;
    delete[] parents;
    return true;
}

void trie::clean_up(trie_node *top) {
    stack<trie_node *> s;
    
    s.push(top);

    // Perform a DFS search and delete any node marked for deletion
    while (!s.empty()) {
        trie_node *current = s.pop();

        current->delete_marked_children();
        current->push_children(&s);

        current->set_add_version(0);
        if (current->get_del_version() != -1) {
            current->set_end_of_word(false);
        }
    }
}

trie_node **trie::get_top_branches(int *size) {
    return _root->get_top_branches(size);
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
    for (int i = 0; i < (int) ngram.size() - 1; i++) {
        static_node *child;
        int index;
        if (current == _root) {
            child = (reinterpret_cast<static_root_node *>(current))->get_child(ngram[i]);
        } else {
            child = current->get_child(ngram[i], &index);
        }
        if (child == nullptr) {
            // If the current trie_node doesn't already contain that child, add it (not as an end of word)
            if (current == _root) {
                current = (reinterpret_cast<static_root_node *>(current))->add_child(ngram[i], false);
            } else {
                current = current->add_child(ngram[i], false, index);
            }
        } else {
            // Otherwise, follow that child's path
            current = child;
        }
    }

    std::string &last_word = ngram.get(ngram.size() - 1);
    static_node *child;
    int index = 0;
    if (current == _root) {
        child = (reinterpret_cast<static_root_node *>(current))->get_child(last_word);
    } else {
        child = current->get_child(last_word, &index);
    }

    if (child != nullptr) {
        // If the word already existed in the tree, we simply mark it as the end of the N-Gram
        child->set_end_of_word(true);
    } else {
        // Otherwise we insert it to that node's children
        if (current == _root) {
            (reinterpret_cast<static_root_node *>(current))->add_child(last_word, true);
        } else {
            current->add_child(last_word, true, index);
        }
    }

}

std::string static_trie::search(const vector<string> &ngram) {
    static_node *current = _root;
    std::stringstream ss , final_ss;
    bloom_filter bf(constants::BLOOM_SIZE, constants::BLOOM_K);
    bool found_one = false;
    bool one_word = false;

    if (_root->empty()) {
        return "$$EMPTY$$";
    }

    for (size_t i = 0; i < ngram.size(); i++) {
        static_node *child = nullptr;
        int k = 0; // Holds the current word index of the current node
        for (size_t j = i; j < ngram.size(); j++) {
            // If k exceeds the number of words in the current node
            // we reset it and continue to its child
            if (child != nullptr && k >= (int) child->lenofwords_size()) {
                k = 0;
                current = child;
            }

            if (k == 0) {
                // If this is the first word of a node, we perform binary search
                if ((child = current->search_static_child(ngram[j])) == nullptr) {
                    break;
                } else {
                    if (one_word) {
                        ss << " " + ngram.at(j);
                    }
                    else {
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


                    // If the current node has more than one word
                    if (child->lenofwords_size() > 1){
                        k++;
                    } else {
                        k = 0;
                        current = child;
                    }
                }
            } else {
                //Traversing the same node.
                if (child->get_word(k) == ngram.at(j)) {
                    if (one_word) {
                        ss << " " + ngram.at(j);
                    } else {
                        ss << ngram.at(j);
                        one_word = true;
                    }

                    if (child->end_of_word(k)) {
                        // Found an ngram
                        if (!bf.check_and_set(ss.str())) {
                            // Ngram did not exist
                            if (found_one) {
                                final_ss << "|";
                            }
                            found_one = true;
                            final_ss << ss.str();
                        }
                    }

                    if (++k >= (int) child->lenofwords_size()) {
                        k = 0;
                        current = child;
                    }
                } else {
                    break;
                }
            }
        }
        ss.str("");
        ss.clear();
        one_word = false;
        current = _root;
    }
    if (!found_one) {
        return "$$END$$";
    }
    else {
        return final_ss.str();
    }
}

void static_trie::compress() {
    static_node *current, *child;
    stack<static_node *> s;
    std::stringstream ss;

    if (_root->empty()) return; //Empty static trie

    _root->push_children(&s);
    while (!s.empty()) {
        current = s.pop();
        if (current->get_st_children_p() == nullptr) {
            current->add_short(current->get_word(), current->is_end_of_word());
            //            current->print_shorts();
            continue;
        }
        ss << current->get_word();
        current->add_short(current->get_word(), current->is_end_of_word());

        while (current->get_children_size() == 1) {
            child = current->get_child(0);
            ss << child->get_word();
            current->add_short(child->get_word(), child->is_end_of_word());
            current->steal_children(child->get_st_children_p());
        }
        current->set_word(ss.str());
        ss.str("");
        ss.clear();
        //        current->print_shorts();
        current->push_children(&s);
    }
}
