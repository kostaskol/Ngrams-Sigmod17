#ifndef TASK_H
#define TASK_H

#include "mvector.hpp"
#include <string>

class trie;

class task {
public:
    task() = default;
    virtual ~task() = default;

    virtual void run() = 0;
};

struct query {
    query(mstd::vector<std::string> &v, int version) : v(std::move(v)), version(version) { }
    query(query &other) : v(std::move(other.v)), version(other.version) { }

    ~query()=default;

    mstd::vector<std::string> v;
    int version;
};

class search_task : public task {
public:
    search_task(trie *t, query &q, int i, std::string *r);

    virtual void run() override;
private:
    trie *t;
    query q;
    int index;
    std::string *results;
};

#endif // TASK_H
