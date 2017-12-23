#include "trie.hpp"
#include "constants.hpp"
#include "minHeap.hpp"
#include "task.hpp"
#include <iostream>
#include <cmd_parser.hpp>
#include <parser.hpp>
#include <unistd.h>
#include <fstream>
#include <helpers.hpp>
#include <thread_pool.hpp>


using std::cout;
using std::cerr;
using std::endl;
using std::string;
using mstd::vector;
using mstd::logger;
using std::ifstream;

void print_and_topk(string *results, int size, size_t topk);

extern void print_help(char *);

int main(int argc, char **argv) {
    // Start command line arguments parsing
    string legal = " -i <init-file> -q <query-file>";
    if (argc < 3) {
        cout << "Invalid number of arguments. Usage: " << argv[0] << legal << endl;
        return 1;
    }

    parser::cmd_args args = parser::parse(argc, argv);

    string init_file = args.init_file;
    string query_file = args.query_file;
    int num_threads = args.num_threads == -1 ? constants::NUM_THREADS : args.num_threads;

    if (init_file.empty()) {
        cerr << "Initialisation file not provided. Exiting" << endl;
        print_help(argv[0]);
        exit(-1);
    }

    if (query_file.empty()) {
        cerr << "Work file not provided. Exiting" << endl;
        print_help(argv[0]);
        exit(-1);
    }

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
        (reinterpret_cast<static_trie *>(t))->compress();
    }
    // End initialisation file parsing

    // Begin query file parsing
    parser query_parser(query_file);

    int cmd_type;

    vector<query> queries(200);
    int version = 1;
    thread_pool tp(num_threads);
    while (true) {
        stop = query_parser.next_command(&v, &cmd_type);
        if (v.size() == 0 && stop) break;
        string s;
        switch (cmd_type) {
            case INSERTION:
                t->add(v);
                break;
            case QUERY: {
                query q(v, version);
                queries.push(q);
                break;
            }
            case DELETION:
                t->delete_ngram(v);
                break;
            case FINISH: {
                string *results = new string[queries.size()];

                for (int i = 0; i < queries.size(); i++) {
                    tp.add_task(new search_task(t, queries.at(i), i, results));
                }
                // Print query results
                tp.wait_all();
                size_t k = 0;
                if (v.size() == 1) {
                    k = (size_t) helpers::to_int(v[0]);
                }
                print_and_topk(results, (int) queries.size(), k);
                break;
            }
        default:
            break;
        }
        if (stop) break;
        v.clear(100);
        version++;
    }
    tp.finish();
    // End query file parsing
    delete t;
}

void print_and_topk(string *results, int size, size_t topk){
    string succ;
    linear_hash_int hashmap;

    for (int i = 0; i < size; i++) {
        succ = results[i];
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