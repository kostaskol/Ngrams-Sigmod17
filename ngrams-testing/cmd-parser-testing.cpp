#include <gtest/gtest.h>
#include <string>
#include "hash_table.hpp"
#include "cmd_parser.hpp"

using mstd::cmd_parser;
using mstd::hash_table;
using std::string;

class CmdParserTest : public ::testing::Test {
public:
    cmd_parser p;
    CmdParserTest()=default;
};

TEST_F(CmdParserTest, basicParsing) {
    // Worst thing ever...
    int argc = 5;
    char **argv = new char*[5];
    argv[0] = new char[7];
    strcpy(argv[0], "ngrams");
    argv[1] = new char[3];
    strcpy(argv[1], "-i");
    argv[2] = new char[5];
    strcpy(argv[2], "init");
    argv[3] = new char[3];
    strcpy(argv[3], "-q");
    argv[4] = new char[6];
    strcpy(argv[4], "query");
    // worst thing ever ends

    hash_table<string> ht;
    ht.put("-i", "<b>");
    ht.put("-q", "<b>");
    ht.put("--debug", "<u>");
    p.parse(argc, argv, ht);
    ASSERT_EQ(p.get_string("-i"), "init");
    ASSERT_EQ(p.get_string("-q"), "query");
    ASSERT_EQ(p.is_set("--debug"), false);
    for (int i = 0; i < 5; i++) {
        delete[] argv[i];
    }
    delete[] argv;
}
