#include <iostream>
#include "trie.h"
#include "mvector.h"
#include "mstring.h"

using namespace std;

int main() {
    cout << "CHanged again" << endl;
    mstd::vector<int *> v;
//    int x = 2;
//    int *p = &x;
//    int x1 = 3;
//    int *p1 = &x1;
//    int x2 = 4;
//    int *p2 = &x;
    int *p = new int(2);
    int *p1 = new int(3);
    int *p2 = new int(4);
    v.push(p);
    v.push(p1);
    v.push(p2);
//    trie t;
//    t.add("Hello World this is an N-Gram");
//    t.add("Hello World this is NOT an N-Gram");
//    mstd::logger::debug("main", "Printing tree:");
//    t.print();
}