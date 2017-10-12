#ifndef TRIE
#define TRIE

#include "mstring.h"
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
        mstd::string _word;
        bool _eow;
        mstd::vector<trie_node *> _children;
        trie_node *_parent;
    public:
        trie_node() :
                _word(""), _eow(false), _parent(nullptr) {
        }

        trie_node(const mstd::string &word, bool eow, trie_node *par = nullptr) :
                _word(word), _eow(eow), _parent(par) {
        }

        trie_node(const trie_node &other)
                : _word(other._word), _eow(other._eow), _children(other._children), _parent(other._parent) {}

        ~trie_node()=default;

        trie_node *add_child(mstd::string word, bool eow) {
            trie_node *new_node = new trie_node(word, eow);
            if (_children.size() == 0) {
                _children.push(new_node);
            } else {
                for (int i = 0; i < (size_t) _children.size(); i++) {
                    if (word > (_children.get(i))->_word) {
                        _children.insert_at(i, new_node);
                        break;
                    }
                }
            }
            return new_node;
        }

        mstd::vector<trie_node *> &get_children() {
            return _children;
        }

        trie_node *get_child(int index) {
            return _children.at(index);
        }

        trie_node *get_child(mstd::string &word) {
            for (int i = 0; i < _children.size(); i++) {
                if (_children.at(i)->_word == word) {
                    return _children.at(i);
                }
            }
            return nullptr;
        }

        bool has_child(mstd::string &word) {
            for (int i = 0; i < _children.size(); i++) {
                if (_children.at(i)->get_word() == word) return true;
            }
            return false;
        }

        void push_child(trie_node *node) {
            _children.push(node);
        }

        bool is_end_of_word() {
            return _eow;
        }

        size_t children_size() {
            return _children.size();
        }

        mstd::string get_word() {
            return _word;
        }

        void set_end_of_word() {
            _eow = true;
        }

        trie_node &operator=(const trie_node &other)=delete;
    };

    trie_node *_root;
    size_t _size;

    void _in_order(trie_node *root);
public:

    trie();
    ~trie();

    void add(mstd::string ngram);

    void print();

    bool search(mstd::string ngram);

    search_result search_one(mstd::string word);
};

#endif // TRIE