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

        auto *rn = dynamic_cast<root_node *>(current);
        if (rn == nullptr) {
            child = current->get_child(ngram.at((size_t) i), &index);
        } else {
            child = rn->get_child(ngram.at((size_t) i));
        }

        if (child == nullptr) {
            // If the current trie_node doesn't already contain that child, add it (not as an end of word)
            if (rn == nullptr) {
                current = current->add_child(ngram.at((size_t) i), false, index, version);
            } else {
                current = rn->add_child(ngram.at((size_t) i), false, version);
            }
        } else {
            // Otherwise, follow that child's path
            current = child;
        }
    }

    std::string &last_word = ngram.get(ngram.size() - 1);
    trie_node *child;
    int index = 0;
    auto *rn = dynamic_cast<root_node *>(current);
    if (rn == nullptr) {
        child = current->get_child(last_word, &index);
    } else {
        child = rn->get_child(last_word);
    }

    if (child != nullptr) {
        // If the word already existed in the tree, we simply mark it as the end of the N-Gram
        child->set_end_of_word(true);
        child->set_add_version(version);
    } else {
        // Otherwise we insert it to that node's children
        if (rn == nullptr) {
            current->add_child(last_word, true, index, version);
        } else {
            rn->add_child(last_word, true, version);
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
            auto *rn = dynamic_cast<root_node *>(current);
            if (rn == nullptr) {
                child = current->get_child(ngram[j], nullptr);
            } else {
                child = rn->get_child(ngram[j]);
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
                if (ss.str() == "the work") {
                    //logger::debug("trie::search", "Found 'the work'");
                    //logger::debug("trie::search", "Found ngram"
                    //        + child->get_word()
                    //        + "\nAdd version: " 
                    //        + std::to_string(child->get_add_version())
                    //        + "\nDel version: "
                    //        + std::to_string(child->get_del_version())
                    //        + "\nCurrent version: "
                    //        + std::to_string(version));

                }
               if (child->is_end_of_word()
                        && child->get_add_version() < version 
                        && (child->get_del_version() == -1
                            || child->get_del_version() > version)) {
                   //if (child->get_word() == "monitoring") {
                   //     logger::debug("trie::search", "Adding monitoring to result");
                   //}
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
        auto *rn = dynamic_cast<root_node *>(current);
        if (rn == nullptr) {
            current = current->get_child(ngram.get((size_t) i), &ch_indexes[i]);
        } else {
            current = rn->get_child(ngram.get((size_t) i));
        };

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
            auto *rn = dynamic_cast<root_node *>(parents[(int) ngram.size() - 1]);
            if (rn == nullptr) {
                //
                parents[(int) ngram.size() - 1]->set_child_del_version(ch_indexes[(int) ngram.size() - 1], version);
            } else {
                rn->set_child_del_version(current->get_word(), version);
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
        // The request N-gram does not exist on the Trie.
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
            auto *rn = dynamic_cast<root_node *>(parents[i]);
            if (rn == nullptr) {
                // Remove child sets version to
                parents[i]->set_child_del_version(ch_indexes[i], version);
            } else {
                rn->set_child_del_version(current->get_word(), version);
            }
            current = parents[i];
        }
    }

    delete[] ch_indexes;
    delete[] parents;
    return true;
}

void trie::clean_up(trie_node *top) {
    // logger::debug("trie::clean_up", "Cleaning branch " + top->get_word());
    if (top->get_del_version() != -1 && top->get_children_size() == 0) {
        // logger::debug("trie::clean_up", "Deleting node " + top->get_word());
        _root->delete_child(top->get_word());
        return;
    }

    vector<trie_node *> chain;
    stack<trie_node *> s;

    s.push(top);

    while (!s.empty()) {
        trie_node *current = s.pop();
        // logger::debug("trie::clean_up", "Popped " + current->get_word() + " from stack");
        current->push_children(&s);

        chain.push(current);
        // logger::debug("trie::clean_up", "Pushed " + current->get_word() + " to chain");
        // If we've reached a leaf node which is an end of word and it HAS been deleted
        if (current->get_children_size() == 0 
                && current->is_end_of_word() /* Redundant check? */
                && current->get_del_version() != -1) {
            // logger::debug("trie::clean_up", "Found eow " + current->get_word());
            for (int i = chain.size() - 1; i > 0; i--) {
                current = chain[i];
//                logger::debug("trie::clean_up", "Current node (" + current->get_word() + ") has " 
//                                        + std::to_string(current->get_children_size()) + " children, is " 
//                                        + (current->is_end_of_word() ? "" : "not") + " an end of word and has "
//                                        + (current->get_del_version() != -1 ? "" : "not") + " been deleted");
//
                // Only delete the current node if it has no children
                // OR if it does have children, it is not an end of word
                // OR if it is an end of word, it has been deleted as well
                if (current->get_children_size() == 0 
                       && (!current->is_end_of_word() 
                           || current->get_del_version() != -1)) {
                                                   
//                  logger::debug("trie::clean_up", "Deleting node " + current->get_word());
                    // TODO: Find a better way to keep track of the child indices
                    // TODO: instead of searching for it again
                    //logger::debug("trie::clean_up", "Deleting node " + current->get_word());
                    //logger::debug("trie::clean_up", "chain before deletion");
                    //for (int j = 0; j < chain.size(); j++) {
                    //    cout << chain[j]->get_word() << " ";
                    //}
                    //cout << endl;
                    chain[i - 1]->delete_child(current->get_word());
                    chain.pop_back();
                    
                    //logger::debug("trie::clean_up", "chain after deletion:");
                    //for (int j = 0; j < chain.size(); j++) {
                    //    cout << chain[j]->get_word() << " ";
                    //}
                    //cout << endl;
                } else {
                    //logger::debug("trie::clean_up", "Not deleting node "
                    //        + current->get_word() 
                    //        + " because it has "
                    //        + std::to_string(current->get_children_size())
                    //        + " children");
                    chain.clear();
                    break;
                }
            }
        }    }

    //    logger::debug("trie::clean_up", "Queue empty");

    if (!top->is_end_of_word() && top->get_children_size() == 0) {
        //        logger::debug("trie::clean_up", "Deleting node " + top->get_word());
        _root->delete_child(top->get_word());
    }

}

//void trie::clean_up(trie_node *top) {
//    // logger::debug("trie::clean_up", "Cleaning branch " + top->get_word());
//    if (top->get_del_version() != -1 && top->get_children_size() == 0) {
//        // logger::debug("trie::clean_up", "Deleting node " + top->get_word());
//        _root->delete_child(top->get_word());
//        return;
//    }
//
//    vector<tuple<trie_node *, int>> chain;
//    stack<tuple<trie_node *, int>> s;
//
//    tuple<trie_node *, int> t(top, 0);
//    s.push(t);
//    //logger::debug("trie::clean_up", "Pushing " + top->get_word() + " to stack");
//
//    while (!s.empty()) {
//        t = s.pop();
//        trie_node *current = t.a;
//        // logger::debug("trie::clean_up", "Popped " + current->get_word() + " from stack");
//        current->push_children(&s);
//
//        chain.push(t);
//        // logger::debug("trie::clean_up", "Pushed " + current->get_word() + " to chain");
//        if (current->get_children_size() == 0 
//                && current->is_end_of_word() 
//                && current->get_del_version() != -1) {
//            // logger::debug("trie::clean_up", "Found eow " + current->get_word());
//            for (int i = chain.size() - 1; i > 0; i--) {
//                tuple<trie_node *, int> tmp = chain[i];
//                current = tmp.a;
//                // Only delete the current node if it has no children
//                // OR if it does have children, it is not an end of word
//                // OR if it is an end of word, it has been deleted as well
//                if (current->get_children_size() == 0 
//                        && (!current->is_end_of_word() 
//                            || current->get_del_version() != -1)) {
//                    //                    logger::debug("trie::clean_up", "Current node (" + current->get_word() + ") has " 
//                    //                            + std::to_string(current->get_children_size()) + " children, is " 
//                    //                            + (current->is_end_of_word() ? "" : "not") + " an end of word and has "
//                    //                            + (current->get_del_version() != -1 ? "" : "not") + " been deleted");
//                    //
//                    //                    logger::debug("trie::clean_up", "Deleting node " + current->get_word());
//                    chain[i - 1].a->delete_child(current->get_word());
//                    chain.pop_back();
//                }
//            }
//        } else {
//            //           logger::debug("trie::clean_up", current->get_word() + " could not be deleted");
//        }
//    }
//
//    //    logger::debug("trie::clean_up", "Queue empty");
//
//    if (!top->is_end_of_word() && top->get_children_size() == 0) {
//        //        logger::debug("trie::clean_up", "Deleting node " + top->get_word());
//        _root->delete_child(top->get_word());
//    }
//
//}
//


trie_node *trie::next_branch() {
    return _root->next_branch();
}

void trie::reset_branch() {
    _root->reset_branch();
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
        auto *rn = dynamic_cast<static_root_node *>(current);
        if (rn == nullptr) {
            child = current->get_child(ngram.at((size_t) i), &index);
        } else {
            child = rn->get_child(ngram.at((size_t) i));
        }
        if (child == nullptr) {
            // If the current trie_node doesn't already contain that child, add it (not as an end of word)
            if (rn == nullptr) {
                current = current->add_child(ngram.at((size_t) i), false, index);
            } else {
                current = rn->add_child(ngram.at((size_t) i), false);
            }
        } else {
            // Otherwise, follow that child's path
            current = child;
        }
    }

    std::string &last_word = ngram.get(ngram.size() - 1);
    static_node *child;
    int index = 0;
    auto *srn = dynamic_cast<static_root_node *>(current);
    if (srn == nullptr) {
        child = current->get_child(last_word, &index);
    } else {
        child = srn->get_child(last_word);
    }

    if (child != nullptr) {
        // If the word already existed in the tree, we simply mark it as the end of the N-Gram
        child->set_end_of_word(true);
    } else {
        // Otherwise we insert it to that node's children
        if (srn == nullptr) {
            current->add_child(last_word, true, index);
        } else {
            srn->add_child(last_word, true);
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
