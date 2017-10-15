#ifndef TRIE
#define TRIE

#include <string>
#include "mvector.h"
#include "logger.h"

class trie_node;

struct search_result {
    bool found;
    bool has_children;
};

class trie {
private:
    class trie_node {
    private:
        std::string _word;
        bool _eow;
        mstd::vector<trie_node> *_children;
        trie_node *_parent;
    public:
        trie_node() :
                _word(""), _eow(false), _parent(nullptr) {
            _children = nullptr;
        }

        trie_node(const std::string &word, bool eow, trie_node *par = nullptr) :
                _word(word), _eow(eow), _parent(par) {
            _children = nullptr;
        }

        trie_node(const trie_node &other)
                  : _word(other._word), _eow(other._eow), _parent(other._parent) {
            if (other._children == nullptr) {
                _children = nullptr;
                return;
            }
            _children = new mstd::vector<trie_node>(*other._children);
        }

        ~trie_node() {
            delete _children;
        }

        trie_node *add_child(std::string word, bool eow) {
            if (_children == nullptr) {
                _children = new mstd::vector<trie_node>();
            }
            trie_node new_node(word, eow, this);
            if (_children->size() == 0) {
                _children->push(new_node);
            } else {
                for (int i = 0; i < (size_t) _children->size(); i++) {
                    if (word > _children->get(i)._word) {
                        _children->insert_at(i, new_node);
                        break;
                    }
                }
            }

            return _children->get_last_inserted();
        }

        mstd::vector<trie_node> &get_children() {
            return *_children;
        }

        trie_node *get_child(int index) {
            return _children->at_p(index);
        }

        trie_node *get_child(std::string &word) {
            if (_children == nullptr) return nullptr;
            for (int i = 0; i < _children->size(); i++) {
                if (_children->at(i)._word == word) {
                    return _children->at_p(i);
                }
            }
            return nullptr;
        }

        void push_child(trie_node *node) {
            if (_children == nullptr) {
                _children = new mstd::vector<trie_node>();
            }
            _children->push(*node);
        }

        bool is_end_of_word() {
            return _eow;
        }

        size_t children_size() {
            return _children->size();
        }

        std::string get_word() {
            return _word;
        }

        void set_end_of_word() {
            _eow = true;
        }

        trie_node &operator=(const trie_node &other) {
            _word = other._word;
            _eow = other._eow;
            _children = other._children;
            _parent = other._parent;
        }
    };

    trie_node *_root;
    size_t _size;

    void _in_order(trie_node *root);
public:

    trie();
    ~trie();

    void add(const std::string &ngram);

    void print();

    bool search(const std::string &ngram);
};

#endif // TRIE