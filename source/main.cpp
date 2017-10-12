#include <iostream>
#include "trie.h"
#include "string.h"

using std::cout;
using std::endl;
using mstd::string;

int main() {
    trie t;
    t.add("Hello World this is an N-Gram");
    t.add("Hello World");
    t.add("Hello");
    string s = t.search("Hello World this is an N-Gram") ? "The N-Gram exists" : "The N-Gram does not exist";
    cout << s << endl;
    s = t.search("Hello") ? "The N-Gram exists" : "The N-Gram does not exist";
    cout << s << endl;
}