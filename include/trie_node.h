#ifndef TRIES_TRIE_NODE_H
#define TRIES_TRIE_NODE_H

class trie_node {
private:
    my_string _word;
    bool _eow;
    my_vector<trie_node *> _children;
public:
    trie_node() :
            _word(""), _eow(false) {}

    trie_node(const my_string word, bool eow) :
            _word(word), _eow(eow) {}
    // No reason for copying of trie_nodes since it will never happen
    trie_node(const trie_node &)=delete;
    ~trie_node()=default;

    trie_node *add_child(my_string word, bool eow) {
        trie_node *new_node = new trie_node(word, eow);
        _children.push(new_node);
        return new_node;
    }

    my_vector<trie_node *> &get_children() {
        return _children;
    }

    trie_node *get_child(int index) {
        return _children.at(index);
    }

    trie_node *get_child(my_string word) {
        for (int i = 0; i < _children.size(); i++) {
            if (_children.at(i)->_word == word) {
                return _children.at(i);
            }
        }
        return nullptr;
    }

    bool has_child(my_string word) {
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

    my_string get_word() {
        return _word;
    }

    trie_node &operator=(const trie_node &other)=delete;
};

#endif //TRIES_TRIE_NODE_H
