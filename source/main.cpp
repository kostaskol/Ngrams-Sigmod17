#include <iostream>
#include <cmd_parser.h>
#include "trie.h"
#include "mstring.h"
#include "hash_table.h"

using std::cout;
using std::endl;
using mstd::string;

int main(int argc, char **argv) {
//    if (argc < 3) {
//        cout << "Invalid number of arguments. Usage: " << argv[0] << " -i <init_file> -q <query_file> [--debug]" << endl;
//        return 1;
//    }
//    mstd::hash_table<string> ht;
//    ht.put("-i", "<s>");
//    ht.put("-q", "<s>");
//    ht.put("--debug", "<none>");
//    mstd::cmd_parser parser;
//    int status = parser.parse(argc, argv, ht);
//    switch (status) {
//        case UNKNOWN_ARG:
//            cout << "Unknown arguments found. Usage: " << argv[0] << " -i <init_file> -q <query_file> [--debug]" << endl;
//            return 1;
//        case WRONG_FORMAT:
//            cout << "Wrong usage found. Usage: " << argv[0] << " -i <init_file> -q <query_file> [--debug]" << endl;
//            return 1;
//        case WRONG_INPUT:
//            std::cerr << "Our input was wrong" << endl;
//            return 1;
//    }
//    string init_file;
//    if (!parser.get_string("-i", &init_file)) {
//        std::cerr << "Something went wrong with the init file" << endl;
//    } else {
//        cout << "Init-File = " << init_file << endl;
//    }
//
//    string query_file;
//    if (!parser.get_string("-q", &query_file)) {
//        std::cerr << "Something went wrong with the query file" << endl;
//    } else {
//        cout << "Query-File = " << query_file << endl;
//    }
//
//    bool debug = parser.is_set("--debug");
//    cout << debug << endl;
    trie t;
    t.add("Hello World this is an N-Gram");
    t.add("Hello World");
    t.add("Hello");
    string s = t.search("Hello World this is an N-Gram") ? "The N-Gram exists" : "The N-Gram does not exist";
    cout << s << endl;
    s = t.search("Hello") ? "The N-Gram exists" : "The N-Gram does not exist";
    cout << s << endl;
    s = t.search("hi") ? "The N-Gram exists" : "The N-Gram does not exist";
    cout << s << endl;
}