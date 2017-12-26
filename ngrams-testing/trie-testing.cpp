#include "trie.hpp"
#include "mvector.hpp"
#include "gtest/gtest.h"
#include "parser.hpp"
#include "helpers.hpp"
#include <string>

using mstd::vector;
using std::string;
using mstd::queue;

class TrieTesting : public ::testing::Test {
public:
    trie t;
    vector<string> v;
    queue<string> results;
    TrieTesting() {};
};

TEST_F(TrieTesting, findAtEmpty){
    string ret;
    helpers::split("This is a test",v,' ');
    t.search(v,&results);
    ASSERT_EQ(results.size(), 1);
    ret = results.pop();
    ASSERT_EQ(ret, "$$EMPTY$$");
}

TEST_F(TrieTesting, findNonPresent){
    string ret;
    helpers::split("This is a test",v,' ');
    t.add(v);
    v.clear();
    helpers::split("This is another test",v,' ');
    t.search(v,&results);
    ret = results.pop();
    ASSERT_EQ(ret, "$$END$$");
}

TEST_F(TrieTesting, findOne){
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

TEST_F(TrieTesting, searchAlreadyFound){
    string ret;
    cmd_parser p("../ngrams-testing/input/medium.input");
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

TEST_F(TrieTesting, deleteFromEmpty) {
    string str = "a b";
    helpers::split(str, v, ' ');
    EXPECT_EQ(t.delete_ngram(v), false);
}

TEST_F(TrieTesting, deleteSimple) {
    string str = "a b";
    helpers::split(str, v, ' ');
    t.add(v);
    EXPECT_EQ(t.delete_ngram(v), true);
}

TEST_F(TrieTesting, deleteMedium) {
    vector<string> v1, v2;
    string str1 = "a b", str2 = "a a";
    helpers::split(str1, v1, ' ');
    t.add(v1);
    helpers::split(str2, v2, ' ');
    t.add(v2);
    EXPECT_EQ(t.delete_ngram(v1), true);
}

TEST_F(TrieTesting, deleteHard) {
    vector<string> v1, v2, v3, v4;
    string str1 = "this is a test";
    string str2 = "this is not a test";
    string str3 = "this is a test and something else";
    string str4 = "something else";

    using helpers::split;
    split(str1, v1, ' ');
    split(str2, v2, ' ');
    split(str3, v3, ' ');
    split(str4, v4, ' ');

    t.add(v1);
    t.add(v2);
    t.add(v3);

    EXPECT_EQ(t.delete_ngram(v1), true);
    EXPECT_EQ(t.delete_ngram(v2), true);
    EXPECT_EQ(t.delete_ngram(v3), true);
    EXPECT_EQ(t.delete_ngram(v4), false);
}

TEST_F(TrieTesting, deleteDouble) {
    vector<string> v;
    string str = "a a";
    helpers::split(str, v, ' ');
    t.add(v);
    EXPECT_EQ(t.delete_ngram(v), true);
    EXPECT_EQ(t.delete_ngram(v), false);
}