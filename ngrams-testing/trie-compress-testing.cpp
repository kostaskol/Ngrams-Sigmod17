#include "trie.hpp"
#include "mvector.hpp"
#include "gtest/gtest.h"
#include "parser.hpp"
#include "helpers.hpp"

using mstd::vector;
using std::string;

class TrieCompressTesting : public ::testing::Test {
public:
    static_trie t;
    vector<string> v;
    TrieCompressTesting () {};
};

TEST_F(TrieCompressTesting, noCompress) {
    helpers::split("This is", v, ' ');
    t.add(v);
    v.clear();
    helpers::split("This isnt", v, ' ');
    t.add(v);
    v.clear();
    t.compress();
    ASSERT_EQ(t.get_num_nodes(), 3);
    ASSERT_EQ(t.get_num_ngrams(), 2);
}

TEST_F(TrieCompressTesting, compressAtRoot) {
    helpers::split("This is", v, ' ');
    t.add(v);
    v.clear();
    helpers::split("This is a", v, ' ');
    t.add(v);
    t.compress();
    EXPECT_EQ(t.get_num_nodes(), 1) << "Didnt compress correctly";
    ASSERT_EQ(t.get_num_ngrams(), 2);
}

TEST_F(TrieCompressTesting, compressExample) {
    parser p("../ngrams-testing/input/static_init.input");
    bool stop = false;
    vector<string> v;
    while (!stop) {
        stop = p.next_init(&v);
        if (v.size() == 0) continue;
        t.add(v);
        v.clear();
    }
    t.compress();
    EXPECT_EQ(t.get_num_nodes(), 8) << "Didnt compress correctly";
    ASSERT_EQ(t.get_num_ngrams(), 6);
}
