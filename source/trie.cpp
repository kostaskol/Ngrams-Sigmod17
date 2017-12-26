#include <binary_search.hpp>
#include "trie.hpp"
#include "bloom_filter.hpp"

using mstd::vector;
using std::string;
using std::cout;
using std::endl;
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
    for (int i = 0; i < ngram.size() - 1; i++) {
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
                else{
                    ss << ngram.at(j);
                    one_word = true;
                }
                if (child->is_end_of_word()) {
                    if (child->get_add_version() < version 
                            || (child->get_del_version() != -1
                                && child->get_del_version() > version)) {
                        if (!bf.check_and_set(ss.str())) {
                        // Ngram did not exist
                            if (found_one) {
                                final_ss << "|";
                            }
                            found_one = true;
                            final_ss << ss.str();
                        }
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

    for (int i = 0; i < ngram.size(); i++) {
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
        if (!current->has_children()) {
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
            // The end_of_word node cannot be deleted, so we un-check the end_of_word flag.
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
        if (current != _root && (current->is_end_of_word() || current->has_children())) {
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

    vector<tuple<trie_node *, int>> chain;
    stack<tuple<trie_node *, int>> s;

    tuple<trie_node *, int> t(top, 0);
    s.push(t);
    //logger::debug("trie::clean_up", "Pushing " + top->get_word() + " to stack");

    while (!s.empty()) {
        t = s.pop();
        trie_node *current = t.a;
        // logger::debug("trie::clean_up", "Popped " + current->get_word() + " from stack");
        current->push_children(&s);
        
        chain.push(t);
        // logger::debug("trie::clean_up", "Pushed " + current->get_word() + " to chain");
        if (current->get_children_size() == 0 
                && current->is_end_of_word() 
                && current->get_del_version() != -1) {
            // logger::debug("trie::clean_up", "Found eow " + current->get_word());
            for (int i = chain.size() - 1; i > 0; i--) {
                tuple<trie_node *, int> tmp = chain[i];
                current = tmp.a;
                // Only delete the current node if it has no children
                // OR if it does have children, it is not an end of word
                // OR if it is an end of word, it has been deleted as well
                if (current->get_children_size() == 0 
                        && (!current->is_end_of_word() 
                        || current->get_del_version() != -1)) {
//                    logger::debug("trie::clean_up", "Current node (" + current->get_word() + ") has " 
//                            + std::to_string(current->get_children_size()) + " children, is " 
//                            + (current->is_end_of_word() ? "" : "not") + " an end of word and has "
//                            + (current->get_del_version() != -1 ? "" : "not") + " been deleted");
//
//                    logger::debug("trie::clean_up", "Deleting node " + current->get_word());
                    chain[i - 1].a->delete_child(tmp.b);
                    chain.pop_back();
                }
            }
        } else {
 //           logger::debug("trie::clean_up", current->get_word() + " could not be deleted");
        }
    }

//    logger::debug("trie::clean_up", "Queue empty");

    if (!top->is_end_of_word() && top->get_children_size() == 0) {
//        logger::debug("trie::clean_up", "Deleting node " + top->get_word());
        _root->delete_child(top->get_word());
    }
      
}

//void trie::clean_up(trie_node *top) {
//    if (top->get_del_version() != -1 && top->get_children_size() == 0) {
//        // TODO: Αν είναι να τα παραλληλοποιήσουμε, πρέπει να υπάρχουν mutexes στο linear hash
//        // since top is a direct child of _root, we simply delete it
//        logger::debug("trie::clean_up", "node " + top->get_word() + " was deleted");
//        _root->delete_child(top->get_word());
//        return;
//    }
//
//    trie_node *current;
//    stack<trie_node *> s;
//
//    s.push(top);
//    top->push_children(&s);
//    vector<vector<trie_node *>> parents;
//    while (!s.empty()) {
//        logger::debug("trie::clean_up", "==Next Node==");
//        bool can_be_deleted = true;
//        current = s.pop();
//        current->set_add_version(0);
//        vector<trie_node *> single_path_parents;
//
//        while (current->get_children_size() == 1) {
//            current->set_add_version(0);
//            // While we can find a single child path we follow it
//            // and keep track of the parents
//            if (current->is_end_of_word()) {
//                if (current->get_del_version() != -1) {
//                    // If it has been deleted, we mark it for removal,
//                    // However, it might not be removed
//                    // (because a child further down the path branches out)
//                    // so we reset its attributes as well
//                    current->set_end_of_word(false);
//                    current->set_del_version(-1);
//                } else {
//                    // If the current node is an end of word of an ngram and has NOT been deleted
//                    // it cannot be removed from the trie
//                    logger::debug("trie::clean_up", "Node " + current->get_word() + " cannot be deleted. Removing path:");
//                    for (int i = 0; i < single_path_parents.size(); i++) {
//                        cout << single_path_parents[i]->get_word() << " ";
//                    }
//                    cout << endl;
//                    can_be_deleted = false;
//                    current->push_children(&s);
//                    break;
//                }
//            }
//
//            logger::debug("trie::clean_up", "Marking node " + current->get_word() + " for deletion");
//            single_path_parents.push(current);
//            current = current->get_child(0);
//        }
//
//        if (!can_be_deleted) {
//            continue;
//        }
//
//        if (current->get_children_size() == 0) {
//            // If we stopped the path traversal due to the absence of children
//            // we push the last node only if it has been deleted
//            if (/* Redundant check? */ current->is_end_of_word() && current->get_del_version() != -1) {
//                single_path_parents.push(current);
//            } else {
//                continue;
//            }
//        } else {
//            // Otherwise, we push the current node's children to the stack
//            if (current->is_end_of_word() && current->get_del_version() != -1) {
//                // We cannot delete this, but we mark it as not end of word 
//                // and reset del version
//                current->set_end_of_word(false);
//                current->set_del_version(-1);
//            }
//            logger::debug("trie::clean_up", "Node " + current->get_word() + " has multiple children");
//            current->push_children(&s);
//            continue;
//        }
//
//        // If the entire above path can be deleted,
//        // we add it to the <to_be_deleted> vector
//        logger::debug("trie::clean_up", "Marking the following path for deletion");
//        for (trie_node *tr : single_path_parents) {
//            cout << tr->get_word() << " ";
//        }
//        cout << endl;
//        parents.push(single_path_parents);
//    }
//
//    for (int i = 0; i < parents.size(); i++) {
//        vector<trie_node *> &v = parents[i];
//        for (int j = (int) v.size() - 2; j >= 0; j--) {
//            // Right now it checks each node's
//            v[j + 1]->delete_child(v[j]->get_word());
//        }
//    }
//
//    // Check if the start of the branch can be deleted again
//    if (top->get_children_size() == 0) {
//        if (top->get_del_version() != -1) {
//            // since top is a direct child of _root, we simply delete it
//            _root->delete_child(top->get_word());
//        }
//    }
//}
//
//void trie::clean_up(trie_node *top) {
//    trie_node *current = top;
//    // We only need to delete the first node in a given chain of
//    // single child nodes
//    stack<trie_node *> to_be_deleted;
//    stack<trie_node *> parents;
//
//    parents.push(current);
//    current->push_children(&parents);
//
//    while (!parents.empty()) {
//        current = parents.pop();
//
//        if (current->get_children_size() == 1) {
//            // We mark all top nodes for deletion
//            // but we only delete them after we've found that
//            to_be_deleted.push(current);
//            current = current->get_child(0);
//            bool has_been_deleted = false;
//            trie_node *child = current;
//            while (true) {
//                if (child->get_children_size() > 1) {
//                    // We cannot delete the parent because a single node path
//                    // to a leaf node does not exist, so we pop it from the stack
//                    to_be_deleted.pop();
//                    break;
//                } else if (child->get_del_version() != -1 && child->get_children_size() == 0) {
//                    has_been_deleted = true;
//                    break;
//                } else if (child->get_children_size() == 0) {
//                    break;
//                }
//
//                child = child->get_child(0);
//            }
//
//            if (!has_been_deleted) {
//                to_be_deleted.pop();
//            }
//        }
//
//        if (current != top) {
//            current->push_children(&parents);
//        }
//
//        // Reset each node's add version to 0
//        // and delete version to -1
//        current->set_add_version(0);
//        if (current->get_del_version() != -1) {
//            current->set_end_of_word(false);
//            current->set_del_version(-1);
//        }
//    }
//
//    while (!to_be_deleted.empty()) {
//        delete to_be_deleted.pop();
//    }
//}

trie_node *trie::next_branch() {
    return _root->next_branch();
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
            if (child != nullptr && k >= child->lenofwords_size()) {
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

                    if (++k >= child->lenofwords_size()) {
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
