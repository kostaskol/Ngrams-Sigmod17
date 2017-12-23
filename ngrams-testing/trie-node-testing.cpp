#include "trie_nodes.hpp"
#include "gtest/gtest.h"
#include <string>

using std::string;

class TrieNodeTest : public ::testing::Test {
public:
    trie_node node;
    static_node s_node;
    TrieNodeTest()=default;
};

TEST_F(TrieNodeTest, insertionTest) {
    string s = "test";
    int index;
    EXPECT_EQ(node.get_child(s, &index), nullptr);
    EXPECT_EQ(index, 0);
    node.add_child(s, false, index);
    EXPECT_NE(node.get_child(s, nullptr), nullptr);
}

TEST_F(TrieNodeTest, correctOrderInsertionTest) {
    string s = "b";
    int index;
    node.get_child(s, &index);
    node.add_child(s, false, index);
    s = "a";
    EXPECT_EQ(node.get_child(s, &index), nullptr);
    EXPECT_EQ(index, 0);

    node.add_child(s, false, index);
    EXPECT_NE(node.get_child(s, &index), nullptr);
    EXPECT_EQ(index, 0);
}

TEST_F(TrieNodeTest, deleteTest) {
    string s = "b";
    int index;
    node.get_child(s, &index);
    node.add_child(s, false, index);
    EXPECT_NE(node.get_child(s, &index), nullptr);

    node.remove_child(index);
    EXPECT_EQ(node.get_child(s, &index), nullptr);
    EXPECT_EQ(index, 0);
}