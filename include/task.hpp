#ifndef TASK_H
#define TASK_H

#include "mvector.hpp"
#include "trie_nodes.hpp"
#include <string>

class trie;
class static_trie;

class task {
public:
    task() = default;
    virtual ~task() = default;

    virtual void run() = 0;
};

struct query {
    query()=default;
    query(mstd::vector<std::string> &v, int version) : v(std::move(v)), version(version) { }
    query(const query &other)=default;
    query(query &&other) noexcept : v(std::move(other.v)), version(other.version) { }

    ~query()=default;

    query &operator=(const query &other)=default;

    query &operator=(query &&other) noexcept {
        v = std::move(other.v);
        version = other.version;
        return *this;
    }

    mstd::vector<std::string> v;
    int version;
};

class search_task : public task {
public:
    search_task(trie *t, query &q, int i, std::string *r);

    void run() override;
private:
    trie *_t;
    query _q;
    int _index;
    std::string *_results;
};

class clean_up_task : public task {
public:
    clean_up_task(trie *t, trie_node *branch);
    clean_up_task(const clean_up_task &other)=delete;
    clean_up_task(clean_up_task &&other) noexcept;

    void run() override;

private:
    trie *_t;
    trie_node *_branch;
};

class compress_task : public task {
public:
    compress_task(static_trie *st, static_node *branch);

    void run() override;

private:
    static_trie *_st;
    static_node *_branch;
};

class insert_task : public task {
public:
    insert_task(trie *t, mstd::vector<std::string> &v, int version);

    insert_task(const insert_task &other)=delete;

    insert_task(insert_task &&other) noexcept;

    void run() override;

private:
    trie *_t;
    mstd::vector<std::string> _v;
    int _version;
};

class deletion_task : public task {
public:
    deletion_task(trie *t, mstd::vector<std::string> &v, int version);

    deletion_task(const deletion_task &other)=delete;

    deletion_task(deletion_task &&other) noexcept;

    void run() override;

private:
    trie *_t;
    mstd::vector<std::string> _v;
    int _version;
};

#endif // TASK_H
