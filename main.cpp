#include <iostream>
#include <trie.h>
#include "my_vector.h"

using namespace std;

int main() {
    trie tr;
    tr.add("hello world");
    tr.add("hello cat");
    tr.add("hello world this is a cat");
    tr.print();
    return 0;
}