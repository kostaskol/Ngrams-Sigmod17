#include "trie.hpp"
#include "mvector.hpp"
#include "gtest/gtest.h"
#include "parser.hpp"
#include <string>
#include <fstream>
#include <iostream>

using mstd::vector;
using mstd::queue;
using std::string;
using std::cout; using std::endl;

class TrieInsertionTesting : public ::testing::Test {
public:
    trie t;
    vector<string> v;
    TrieInsertionTesting() {
        t;
        v;
    }
};

TEST_F(TrieInsertionTesting, insertEasy) {
    v.push("a"); v.push("b"); v.push("c"); v.push("d");
    t.add(v);
    queue<string> results;
    t.search(v, &results);
    EXPECT_EQ(results.pop(), "a b c d");
}

TEST_F(TrieInsertionTesting, insertEasyF) {
    v.push("a"); v.push("b"); v.push("c"); v.push("d");
    t.add(v);
    queue<string> results;
    v.remove_at(v.size() - 1);
    for (const string &s : v) {
        cout << s << endl;
    }
    t.search(v, &results);
    EXPECT_EQ(results.pop(), "$$END$$");
}

TEST_F(TrieInsertionTesting, insertMed) {
    parser p("../ngrams-testing/input/medium.input");
    bool stop = false;
    vector<string> v;
    while (!stop) {
        stop = p.next_init(&v);
        if (v.size() == 0) continue;
        t.add(v);
        v.clear();
    }

}

TEST_F(TrieInsertionTesting, insertLarge) {
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
