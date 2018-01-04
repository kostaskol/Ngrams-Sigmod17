#include "bloom_filter.hpp"
#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <fstream>

using std::string;
using std::ifstream;
using std::cout;
using std::endl;

#define SIZE 4096
#define K 11

const string DIR = "../ngrams-testing/test-input/bloom-filter-input/";

class BloomFilterTest : public ::testing::Test {
public:
    ifstream inp;
    bloom_filter bf;
    BloomFilterTest() : bf(SIZE, K) {}
};

TEST_F(BloomFilterTest, falsePositiveTest) {
    string files[] = {"small", "medium", "large", "extra-large"};
    for (const string &s : files) {
        string tmp = DIR + s + ".bloom.correct";
        inp.open(tmp);
        string line;
        int size = 0;
        while (std::getline(inp, line)) {
            bf.insert(line);
            size++;
        }

        inp.close();
        inp.open(DIR + s + ".bloom.wrong");
        int false_positives = 0;
        while(std::getline(inp, line)) {
            if (bf.check(line)) {
                false_positives++;
            }
        }

        inp.close();
    }

}

TEST_F(BloomFilterTest, simpleTest) {
    bf.insert("Hello World");
    EXPECT_EQ(bf.check("Hello World"), true);
    EXPECT_EQ(bf.check("Hello World!"), false);
}
