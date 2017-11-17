#include <iostream>
#include "bloom_filter.hpp"
#include <cmd_parser.hpp>
#include <parser.hpp>
#include "trie.hpp"
#include <unistd.h>
#include "bit_vector.hpp"
#include <fstream>
using std::cout;
using std::endl;
using std::string;
using mstd::vector;
using mstd::logger;
using std::ifstream;
std::string mstd::logger::file;

#define SIZE 1024
#define K 11

int main(int argc, char **argv) {
    ifstream s("/home/kostas/CLionProjects/ngrams/ngrams-testing/test-input/bloom-filter-input/medium.bloom.correct");
    bloom_filter bf(SIZE, K);
    string line;
    while (std::getline(s, line)) {
        cout << bf.check(line) << endl;
        bf.insert(line);
        cout << bf.check(line) << endl;
    }
//    vector<size_t> indexes;
//    for (size_t i = 0; i < 1000; i++) {
//        if ((i % 2 == 0 || i % 3 == 0 || i % 5 == 0) && !bv.test_and_set(i)) {
//            indexes.push(i);
//        }
//    }
//
//    if (indexes.size() != 0) {
//        cout << "There were " << indexes.size() << " errors" << endl;
//        cout << "at indexes: ";
//        for (size_t i : indexes) {
//            cout << i << ", ";
//        }
//    } else {
//        cout << "No errors found !" << endl;
//    }
//    logger::file = "../debug/log.txt";
//    // Start command line arguments parsing
//    string legal = " -i <init-file> -q <query-file> [--debug]";
//    if (argc < 3) {
//        cout << "Invalid number of arguments. Usage: " << argv[0] << legal << endl;
//        return 1;
//    }
//
//    auto *ht = new mstd::hash_table<string>(3);
//    ht->put("-i", "<b>");
//    ht->put("-q", "<b>");
//    ht->put("--debug", "<u>");
//    auto *c_parser = new mstd::cmd_parser(true, std::move(legal));
//    c_parser->parse(argc, argv, *ht);
//    delete ht;
//
//    string init_file;
//    string query_file;
//    bool debug;
//    try {
//        init_file = c_parser->get_string("-i");
//        query_file = c_parser->get_string("-q");
//    } catch (std::runtime_error &e) {
//        logger::error("main", "User has not provided both -i and -q. Exiting..", BOTH);
//        return -1;
//    }
//
//    debug = c_parser->is_set("--debug");
//
//    delete c_parser;
//    // End command line arguments parsing
//
//    trie t;
//
//    // Begin initialisation file parsing
//    parser init_parser(init_file);
//
//    vector<string> v;
//    while (true) {
//        bool stop = init_parser.next_init(&v);
//        if (v.size() == 0 && stop) break;
//        t.add(v);
////        logger::success("init", "Added N-Gram " + s);
//        v.clear();
//        if (stop) break;
//    }
//
//    // End initialisation file parsing
//
//    // Begin query file parsing
//    parser query_parser(query_file);
//
//    int cmd_type;
//
//    mstd::queue<std::string> results;
//    while (true) {
//        bool stop = query_parser.next_command(&v, &cmd_type);
//        if (v.size() == 0 && stop) break;
//        string s;
//        switch (cmd_type) {
//            case INSERTION:
//                t.add(v);
//                break;
//            case QUERY:{
//                t.search(v,&results);
//                break;
//            }
//            case DELETION:
//                t.delete_ngram(v);
//                break;
//            case FINISH:
//                // Print query results
//                string succ = "";
//                while(!results.empty()){
//                    succ = results.pop();
//                    if (succ == "$$END$$") {
//                        std::cout << "-1" << '\n';
//                    }
//                    else{
//                        std::cout << succ << '\n';
//                    }
//                }
//                break;
//        }
//        if (stop) break;
//        v.clear();
//    }
//    results.clear();
//    // End query file parsing
}
