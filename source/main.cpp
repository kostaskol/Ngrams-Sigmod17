#include "trie.hpp"
#include "constants.hpp"
#include "minHeap.hpp"
#include <iostream>
#include <cmd_parser.hpp>
#include <parser.hpp>
#include <unistd.h>
#include <fstream>


using std::cout;
using std::endl;
using std::string;
using mstd::vector;
using mstd::logger;
using std::ifstream;

void print_and_topk(mstd::queue<std::string> *results, size_t topk);

int main(int argc, char **argv) {
    // Start command line arguments parsing
    string legal = " -i <init-file> -q <query-file>";
    if (argc < 3) {
        cout << "Invalid number of arguments. Usage: " << argv[0] << legal << endl;
        return 1;
    }

    auto *ht = new mstd::hash_table<string>(3);
    ht->put("-i", "<b>");
    ht->put("-q", "<b>");
    auto *c_parser = new mstd::cmd_parser(true, std::move(legal));
    c_parser->parse(argc, argv, *ht);
    delete ht;

    string init_file;
    string query_file;
    try {
        init_file = c_parser->get_string("-i");
        query_file = c_parser->get_string("-q");
    } catch (std::runtime_error &e) {
        logger::error("main", "User has not provided both -i and -q. Exiting..");
        return -1;
    }

    delete c_parser;
    // End command line arguments parsing

    vector<string> v(100);
    trie* t;

    // Begin initialisation file parsing
    parser init_parser(init_file);

    bool stop = init_parser.next_init(&v);
    bool compress = false;
    if (v.get(0) == "STATIC") {
        t = new static_trie();
        compress = true;
    } else if (v.get(0) == "DYNAMIC") {
        t = new trie();
    }
    v.clear(100);

    while (true) {
        stop = init_parser.next_init(&v);
        if (v.size() == 0 && stop) break;
        t->add(v);
        v.clear(100);
        if (stop) break;
    }
    if (compress) {
        t->compress();
    }
    // End initialisation file parsing

    // Begin query file parsing
    parser query_parser(query_file);

    int cmd_type;

    mstd::queue<std::string> results;
    while (true) {
        stop = query_parser.next_command(&v, &cmd_type);
        if (v.size() == 0 && stop) break;
        string s;
        switch (cmd_type) {
            case INSERTION:
                t->add(v);
                break;
            case QUERY:
                t->search(v,&results);
                break;
            case DELETION:
                t->delete_ngram(v);
                break;
            case FINISH: {
                // Print query results
                size_t k = 0;
                if (v.size() == 1) {
                    k = helpers::to_int(v[0]);
                }
                print_and_topk(&results, k);
                break;
            }
        default:
            break;
        }
        if (stop) break;
        v.clear(100);
    }
    results.clear();
    // End query file parsing
    delete t;
}

void print_and_topk(mstd::queue<std::string> *results, size_t topk){
    string succ = "";
    linear_hash_int hashmap;

    while(!results->empty()){
        succ = results->pop();
        if (succ == "$$END$$" || succ == "$$EMPTY$$") {
            std::cout << "-1" << '\n';
        }
        else{
            std::cout << succ << '\n';
            if (topk) {                                    // if topk == 0, no topk operation
                vector<string> answers;
                helpers::split(succ, answers, '|');
                for (size_t i = 0; i < answers.size(); i++) {
                    hashmap.insert(answers[i]);
                }
            }
        }
    }
    if ((topk > 0) && (!hashmap.empty())) {  //gia na sigoureutoume oti den brhke mono -1 stis apanthseis
        std::cout << "Top: ";

        size_t max_freq = hashmap.get_max();
        vector<pair> *array = new vector<pair>[max_freq];

        hashmap.fill_with_items(array);

        int counter = 0;
        bool one_word = false;
        for (int i = max_freq-1; i >= 0 && counter != topk; i--) {
            minHeap heap(array[i]);
            heap.heapSort();
            for (int j = 0; j < array[i].size() && counter != topk; j++) {
                counter++;
                if (one_word) {
                    std::cout << "|";
                }
                std::cout << array[i].at(j).get_word();
                one_word = true;
            }
        }
        std::cout << '\n';

        delete[] array;
    }

}