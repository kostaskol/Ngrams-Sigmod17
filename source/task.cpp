#include "task.hpp"
#include "mvector.hpp"
#include "trie.hpp"
#include <string>

using mstd::vector;
using std::string;

search_task::search_task(trie *t, query &q, int i, std::string *r) : q(q), index(i), results(r) { }

void search_task::run() {

    static_trie *st = dynamic_cast<static_trie *>(t);

    string result;
    if (st == nullptr) {
        // t was a trie
        result = t->search(q.v, q.version);
    } else {
        // t was a static trie
        result = st->search(q.v);
    }

    results[index] = std::move(result);
    delete this;
}