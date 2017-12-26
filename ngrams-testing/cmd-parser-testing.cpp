#include <gtest/gtest.h>
#include <string>
#include "cmd_parser.hpp"

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


}
