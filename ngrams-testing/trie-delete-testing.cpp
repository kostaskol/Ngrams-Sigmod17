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

}