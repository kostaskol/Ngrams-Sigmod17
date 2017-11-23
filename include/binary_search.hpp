#include "trie_node.hpp"
#include "mvector.hpp"
#include <string>

inline bool bsearch_children(const std::string &word, const mstd::vector<trie_node> &children, int *index) {
    // New insertion
    if (children.size() == 0) {
        *index = 0;
        return false;
    }
    if (children[0].get_word() > word) {
        *index = 0;
        return false;
    }
    else if (children[0].get_word() == word) {
        *index = 0;
        return true;
    }

    if (children[children.size() - 1].get_word() < word) {
        *index = (int) children.size();
        return false;
    }
    else if (children[children.size() - 1].get_word() == word) {
        *index = (int)children.size() - 1;
        return true;
    }

    int left = 0;
    int right = (int) children.size() - 1;
    while (left <= right) {
        int mid = left + ((right-left) / 2);

        if (children[(size_t) mid].get_word() == word) {
            *index = mid;
            return true;
        }

        if (left == right || left == right - 1) {
            if (children[(size_t) left].get_word() > word ) {
                *index = left;
            }
            else if (children[(size_t) right].get_word( )< word) {
                *index = right + 1;
            }
            else if (children[(size_t) right].get_word() > word) {
                *index = right;
            }
            else {
                if (children[(size_t) right].get_word() == word) {
                    *index = right;
                } else {
                    *index = mid;
                }
                return true;
            }
            return false;
        }
        if (children[(size_t) mid].get_word() > word) {
            right = mid - 1;
        }
        else if (children[(size_t) mid].get_word() < word) {
            left = mid + 1;
        }
    }
}
