#include "trie.hpp"
#include "mvector.hpp"
#include "gtest/gtest.h"
#include "parser.hpp"
#include <string>
#include <fstream>
#include <iostream>

using mstd::vector;
using std::string;

class TrieDeletionionTesting : public ::testing::Test {
public:
    trie t;
    vector<string> v;
    TrieDeletionionTesting() {
        t;
        v;
    }
};

TEST_F(TrieDeletionionTesting, deleteEasy) {
    v.push("a"); v.push("b"); v.push("c"); v.push("d");
    t.add(v);
    t.delete_ngram(v);
    string trie_str = t.to_string();
    string proper = "";
    EXPECT_EQ(trie_str, proper);
}

TEST_F(TrieDeletionionTesting, deleteMed) {
    parser p("/home/sukee/Dropbox/Project/ngrams-testing/input/medium.input");
    bool stop = false;
    vector<string> v;
    while (!stop) {
        stop = p.next_init(&v);
        if (v.size() == 0) continue;
        t.add(v);
        v.clear();
    }
    // remove an N-gram that is a prefix of other N-gram.
    v.push("this"); v.push("is"); v.push("a");
    t.delete_ngram(v);
    v.clear();
    // remove N-grams that can delete some of their grams.
    v.push("this"); v.push("is"); v.push("a"); v.push("cat");
    t.delete_ngram(v);
    v.clear();
    v.push("this"); v.push("is"); v.push("a"); v.push("dog");
    t.delete_ngram(v);
    v.clear();
    v.push("this"); v.push("is"); v.push("cat");
    t.delete_ngram(v);
    v.clear();
    v.push("that"); v.push("is"); v.push("a"); v.push("slow"); v.push("dog");
    t.delete_ngram(v);
    v.clear();
    // remove an N-gram that does not exists.
    v.push("that"); v.push("ngram"); v.push("does"); v.push("not"); v.push("exist");
    t.delete_ngram(v);
    v.clear();

    string output = t.to_string();
    std::ofstream out("/home/sukee/Dropbox/Project/ngrams-testing/output/output-d-medium.test");
    out << output;
    std::ifstream in("/home/sukee/Dropbox/Project/ngrams-testing/input/output-d-medium.proper");
    std::stringstream proper_output;
    std::string line;
    while (std::getline(in, line)) {
        proper_output << line << "\n";
    }
    EXPECT_EQ(proper_output.str(), output);
}

TEST_F(TrieDeletionionTesting, deleteLarge) {
    parser p("/home/sukee/Dropbox/Project/ngrams-testing/input/large.input");
    bool stop = false;
    vector<string> v;
    while (!stop) {
        stop = p.next_init(&v);
        if (v.size() == 0) continue;
        t.add(v);
        v.clear();
    }
    v.push("a"); v.push("word");
    t.delete_ngram(v);
    v.clear();
    // remove an N-gram that doesn't exist.
    v.push("and"); v.push("then"); v.push("we");
    t.delete_ngram(v);
    v.clear();
    v.push("and"); v.push("then"); v.push("we"); v.push("need"); v.push("to");
    t.delete_ngram(v);
    v.clear();
    v.push("because"); v.push("the"); v.push("tests"); v.push("would");
    t.delete_ngram(v);
    v.clear();
    v.push("fail");
    t.delete_ngram(v);
    v.clear();
    v.push("when"); v.push("they"); v.push("shouldn't");
    t.delete_ngram(v);
    v.clear();

    string output = t.to_string();
    std::ofstream out("/home/sukee/Dropbox/Project/ngrams-testing/output/output-d-large.test");
    out << output;
    std::ifstream in("/home/sukee/Dropbox/Project/ngrams-testing/input/output-d-large.proper");
    std::stringstream proper_output;
    std::string line;
    while (std::getline(in, line)) {
        proper_output << line << "\n";
    }
    EXPECT_EQ(proper_output.str(), output);
}