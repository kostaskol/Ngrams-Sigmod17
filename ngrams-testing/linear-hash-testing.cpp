#include "linear_hash.hpp"
#include "trie_nodes.hpp"
#include "mvector.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <gtest/gtest.h>

using mstd::vector;
using std::ifstream;
using std::string;
using std::cout;
using std::endl;

class LinearHashTest : public ::testing::Test {
public:
	linear_hash lh;
	LinearHashTest() { }
};

TEST_F(LinearHashTest, InsertAndGetTest) {
    string s = "some";
    lh.insert(s, false);
    s = "test";
    lh.insert(s, false);
    s = "ngram";
    lh.insert(s, false);
    
    trie_node *tmp = lh.get("some");
    EXPECT_NE(tmp, nullptr);
    
    tmp = lh.get("ngram");
    EXPECT_NE(tmp, nullptr);
    
    tmp = lh.get("test");
    EXPECT_NE(tmp, nullptr);
    
    tmp = lh.get("N/A");
    EXPECT_EQ(tmp, nullptr);
}

TEST_F(LinearHashTest, SizeIncTest) {
    string strs[] = { "hello", "world", "let's", "add", "some", "words", "and", "then", "more", "words", "I", "am", "writing", "a", "small", "story", "because", "i", "need", "as", "many", "words", "as", "iI", "can", "get" };
    string wrong_strs[] = { "These", "Words", "Certainly", "Do", "Not", "Exist", "Because", "They", "Are", "Capitalized" };
    
    for (string s : strs) {
        lh.insert(s, false);
    }
    
    for (string s : strs) {
        trie_node *tmp = lh.get(s);
        EXPECT_NE(tmp, nullptr);
    }
    
    for (string s : wrong_strs) {
        trie_node *tmp = lh.get(s);
        EXPECT_EQ(tmp, nullptr);
    }
}


TEST_F(LinearHashTest, FalsePositiveTest) {
    string strs[] = { "Some", "Values", "For", "Initialization" };
    for (auto s : strs) {
        lh.insert(s, false);
    }
    
    ifstream wrong_in("../ngrams-testing/test-input/linear-hash-input/big_file.wrong");
    string s;
    while (std::getline(wrong_in, s)) {
        trie_node *tmp = lh.get(s);
        EXPECT_EQ(tmp, nullptr);
    }
    
    wrong_in.close();
}

/*
 * Easy -> 50k words
 * Medium -> 120k words
 * Hard -> 260k words
 */

TEST_F(LinearHashTest, BigFileEasyTest) {
    ifstream in("../ngrams-testing/test-input/linear-hash-input/big_file.correct");
    
    string s;
    for (int i = 0; i < 50000 && std::getline(in, s); i++) {
        lh.insert(s, false);
    }
    
    in.clear();
    in.seekg(0, std::ios::beg);
    
    for (int i = 0; i < 50000 && std::getline(in, s); i++) {
        trie_node *tmp = lh.get(s);
        EXPECT_NE(tmp, nullptr);
    }
    
    in.close();
}

TEST_F(LinearHashTest, BigFileMediumTest) {
    ifstream in("../ngrams-testing/test-input/linear-hash-input/big_file.correct");
    
    string s;
    for (int i = 0; i < 120000 && std::getline(in, s); i++) {
        lh.insert(s, false);
    }
    
    in.clear();
    in.seekg(0, std::ios::beg);
    
    for (int i = 0; i < 120000 && std::getline(in, s); i++) {
        trie_node *tmp = lh.get(s);
        EXPECT_NE(tmp, nullptr);
    }
    
    in.close();
}

TEST_F(LinearHashTest, BigFileHardTest) {
    ifstream in("../ngrams-testing/test-input/linear-hash-input/big_file.correct");
    
    string s;
    while (std::getline(in, s)) {
        lh.insert(s, false);
    }
    
    in.clear();
    in.seekg(0, std::ios::beg);
    
    while (std::getline(in, s)) {
        trie_node *tmp = lh.get(s);
        EXPECT_NE(tmp, nullptr);
    }
    
    in.close();
}

TEST_F(LinearHashTest, BigFileHardFalsePositiveTest) {
    ifstream correct_in("../ngrams-testing/test-input/linear-hash-input/big_file.correct");
    ifstream wrong_in("../ngrams-testing/test-input/linear-hash-input/big_file.wrong");
    
    string s;
    while (std::getline(correct_in, s)) {
        lh.insert(s, false);
    }
    
    while (std::getline(wrong_in, s)) {
        trie_node *tmp = lh.get(s);
        EXPECT_EQ(tmp, nullptr);
    }
    
    correct_in.close();
    wrong_in.close();
}

TEST_F(LinearHashTest, BasicDeletionTest) {
    string s = "hello";
    lh.insert(s, false);
    
    s = "world";
    
    lh.insert(s, false);
    
    s = "hello";
    
    lh.delete_word(s);
    
    trie_node *tmp = lh.get(s);
    
    EXPECT_EQ(tmp, nullptr);    

    s = "world";
    
    tmp = lh.get(s);
    EXPECT_NE(tmp, nullptr);
}

TEST_F(LinearHashTest, MultipleDeletionTest) {
    ifstream in("../ngrams-testing/test-input/linear-hash-input/big_file-no_dups.correct");
    
    string s;
    
    while (std::getline(in, s)) {
        lh.insert(s, false);
    }
    
    in.clear();
    in.seekg(0, std::ios::beg);
    
    // Delete every second word
    for (int i = 0; std::getline(in, s); i++) {
        if (i % 2 == 0) lh.delete_word(s);
    }
    
    in.clear();
    in.seekg(0, std::ios::beg);
    
    for (int i = 0; std::getline(in, s); i++) {
        trie_node *tmp = lh.get(s);
        
        if (i % 2 == 0) {
            EXPECT_EQ(tmp, nullptr);
        } else {
            EXPECT_NE(tmp, nullptr);
        }
    }
}


