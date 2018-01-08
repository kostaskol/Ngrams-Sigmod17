
#include "parser.hpp"
#include "mvector.hpp"
#include <string>
#include <gtest/gtest.h>

using mstd::vector;
using std::string;

class ParserTest : public testing::Test {
public:
    parser p;
    vector<string> outp;
    ParserTest() : p("../ngrams-testing/input/parser-tests.txt"){}
};


TEST_F(ParserTest, parsingTest) {
    vector<std::string> v;
    v.push("Hello");
    v.push("World");
    p.next_init(&outp);
    // The v and outp vectors should be equal (create an operator== ?
    // (would force us to override the operator== for each custom class))
    EXPECT_EQ(outp.size(), 2);
    EXPECT_EQ(v[0], outp[0]);
    EXPECT_EQ(v[1], outp[1]);
}

TEST_F(ParserTest, exceptionTest) {
    // First call should return true since the file only has one line
    EXPECT_EQ(p.next_init(&outp), true);
    // And a second call should throw a runtime_error
    try {
        p.next_init(&outp);
    } catch (std::runtime_error &e) {
        SUCCEED() << "Runtime error successfully thrown";
    } catch (...) {
        FAIL() << "Unknown exception thrown";
    }
}
