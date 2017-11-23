#include "linear_hash.hpp"
#include "trie_node.hpp"
#include <string>

using namespace std;

bool __debug__ = true;

int main() {
    string strs[] = { "hello", "world", "let's", "add", "some", "words", "and", "then", "more", "words", "I", "am", "writing", "a", "small", "story", "because", "i", "need", "as", "many", "words", "as", "iI", "can", "get" };
    string strs_bak[] =  { "hello", "world", "let's", "add", "some", "words", "and", "then", "more", "words", "I", "am", "writing", "a", "small", "story", "because", "i", "need", "as", "many", "words", "as", "iI", "can", "get" };
    string wrongs[] = { "this", "im", "hoping", "did", "not", "inserted" };
    linear_hash lh;
    for (string &s : strs) {
        lh.insert(s, false);
    }
    
    lh.print();
    
    for (string &s : strs_bak) {
        trie_node *tmp = lh.get(s);
        if (tmp == nullptr) {
            cerr << s << " was inserted but not found" << endl;
            exit(-1);
        } else {
            cout << s << " was inserted and found" << endl;
        }
    }
    
    for (string &s : wrongs) {
        trie_node *tmp = lh.get(s);
        if (tmp == nullptr) {
            cout << s << " was not inserted and was not found" << endl;
        } else {
            cout << s << " was not inserted but found" << endl;
            exit(-1);
        }
    }
}
