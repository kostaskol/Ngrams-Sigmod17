#include "task.hpp"
#include "logger.hpp"
#include "trie.hpp"
#include <string>

using mstd::logger;
using std::string;
using mstd::vector;

/*
 * Search task implementation
 */
search_task::search_task(trie *t, query &q, int i, std::string *r) : _t(t), _q(q), _index(i), _results(r) { }

void search_task::run() {

    auto *st = dynamic_cast<static_trie *>(_t);

    string result;
    if (st == nullptr) {
        // t was a trie
        _results[_index] = _t->search(_q.v, _q.version);
    } else {
        // t was a static trie
        _results[_index] = st->search(_q.v);
    }

    // _results[_index] = std::move(result);
    delete this;
}

/*
 * Clean up task implementation
 */
clean_up_task::clean_up_task(trie *t, trie_node *branch) : _t(t), _branch(branch) { }

void clean_up_task::run() {
    _t->clean_up(_branch);
    delete this;
}

/*
 * Compress task implementation
 */
compress_task::compress_task(static_trie *st, static_node *branch) : _st(st), _branch(branch) { }

void compress_task::run() {
    _st->compress(_branch);
    delete this;
}

/*
 * Insertion task implementation
 */

insert_task::insert_task(trie *t, vector<string> &v, int version) : _t(t), _v(std::move(v)), _version(version) { }

void insert_task::run() {
    _t->add(_v, _version);
    delete this;
}

/*
 * Deletion task implementation
 */

deletion_task::deletion_task(trie *t, vector<string> &v, int version) : _t(t), _v(std::move(v)), _version(version) { }

void deletion_task::run() {
    _t->delete_ngram(_v, _version);
    delete this;
}

/*
 * TopK task implementation
 */

topk_task::topk_task(string *results, int res_size, linear_hash_int *hashmap, int id, int threads) : _results(results), _res_size(res_size), _hashmap(hashmap), _id(id), _threads(threads) { }

void topk_task::run() {
    int start, end;

    start = _id*(_res_size / _threads);

    if ( (_res_size % _threads > 0) && (_id == _threads - 1) ) {
        end = start + _res_size / _threads + _res_size % _threads;
    }
    else {
        end = start + _res_size / _threads;
    }

    for (size_t i = start; i < end; i++) {
        vector<string> answers;
        helpers::split(_results[i], answers, '|');
        for (size_t j = 0; j < answers.size(); j++) {
            _hashmap->insert(answers[j]);
        }
    }

    delete this;
}
