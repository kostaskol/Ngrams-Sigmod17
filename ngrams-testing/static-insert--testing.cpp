#include "trie.hpp"
#include "mvector.hpp"
#include "gtest/gtest.h"
#include "parser.hpp"
#include <string>
#include <fstream>
#include <iostream>

using mstd::vector;
using std::string;

class StaticInsertionTesting : public ::testing::Test {
public:
    static_trie t;
    vector<string> v;
    StaticInsertionTesting() {
        t;
        v;
    }
};

TEST_F(StaticInsertionTesting, insertEasy) {
    v.push("a"); v.push("b"); v.push("c"); v.push("d");
    t.add(v);
    t.compress();
    string trie_str = t.to_string();
    string proper = "a b c d\n";
    EXPECT_EQ(trie_str, proper);
}

TEST_F(StaticInsertionTesting, insertMed) {
    parser p("../ngrams-testing/input/medium.input");
    bool stop = false;
    vector<string> v;
    while (!stop) {
        stop = p.next_init(&v);
        if (v.size() == 0) continue;
        t.add(v);
        v.clear();
    }
    string output = t.to_string();
    std::ofstream out("../ngrams-testing/output/output-medium.test");
    out << output;
    std::ifstream in("../ngrams-testing/input/output-medium.proper");
    std::stringstream proper_output;
    std::string line;
    while (std::getline(in, line)) {
        proper_output << line << "\n";
    }
    EXPECT_EQ(proper_output.str(), output);
}

TEST_F(StaticInsertionTesting, insertLarge) {
    parser p("../ngrams-testing/input/large.input");
    bool stop = false;
    vector<string> v;
    while (!stop) {
        stop = p.next_init(&v);
        if (v.size() == 0) continue;
        t.add(v);
        v.clear();
    }
    string output = t.to_string();
    std::ofstream out("../ngrams-testing/output/output-large.test");
    out << output;
    std::ifstream in("../ngrams-testing/input/output-large.proper");
    std::stringstream proper_output;
    std::string line;
    while (std::getline(in, line)) {
        proper_output << line << "\n";
    }
    EXPECT_EQ(proper_output.str(), output);
}
