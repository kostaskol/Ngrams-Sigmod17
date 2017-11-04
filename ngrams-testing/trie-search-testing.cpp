#include "trie.hpp"
#include "mvector.hpp"
#include "gtest/gtest.h"
#include "parser.hpp"
#include "helpers.hpp"
#include <string>

using mstd::vector;
using std::string;
using mstd::queue;

class TrieSearchTesting : public ::testing::Test {
public:
    trie t;
    vector<string> v;
    queue<string> results;
    TrieSearchTesting() {};
};

TEST_F(TrieSearchTesting, findAtEmpty){
    string ret;
    helpers::split("This is a test",v,' ');
    t.search(v,&results);
    ASSERT_EQ(results.size(), 1);
    ret = results.pop();
    ASSERT_EQ(ret, "$$EMPTY$$");
}

TEST_F(TrieSearchTesting, findNonPresent){
    string ret;
    helpers::split("This is a test",v,' ');
    t.add(v);
    v.clear();
    helpers::split("This is another test",v,' ');
    t.search(v,&results);
    ret = results.pop();
    ASSERT_EQ(ret, "$$END$$");
}

TEST_F(TrieSearchTesting, findOne){
    string ret;
    helpers::split("This is a test",v,' ');
    t.add(v);
    v.clear();
    helpers::split("That is another test",v,' ');
    t.add(v);
    t.search(v,&results);
    ret = results.pop();
    ASSERT_EQ(ret, "That is another test");
}

TEST_F(TrieSearchTesting, searchAlreadyFound){
    string ret;
    parser p("../ngrams-testing/input/medium.input");
    bool stop = false;
    vector<string> v;
    while (!stop) {
        stop = p.next_init(&v);
        if (v.size() == 0) continue;
        t.add(v);
        v.clear();
    }
    helpers::split("this is a this is a fast car and something else",v,' ');
    t.search(v,&results);
    EXPECT_EQ(results.size(), 1) << "More than one final answers";
    ret = results.pop();
    ASSERT_EQ(ret, "this is a|this is a fast car");
}
