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

int main(int argc, char **argv) {
    // Start command line arguments parsing

    if (argc < 3) {
        cout << "Invalid number of arguments" << endl;
        cmd_parser::print_help(argv[0]);
        return 1;
    }

    cmd_parser::cmd_args args = cmd_parser::parse(argc, argv);

    string init_file = args.init_file;
    string query_file = args.query_file;
    int num_threads = args.num_threads == -1 ? constants::NUM_THREADS : args.num_threads;

    if (init_file.empty()) {
        cerr << "Initialisation file not provided. Exiting" << endl;
        cmd_parser::print_help(argv[0]);
        exit(-1);
    }

    if (query_file.empty()) {
        cerr << "Work file not provided. Exiting" << endl;
        cmd_parser::print_help(argv[0]);
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
        if (compress) {
            (dynamic_cast<static_trie *>(t))->add(v);
        } else {
            t->add(v);
        }

        v.clear(100);
        if (stop) break;
    }

    if (compress) {
        (dynamic_cast<static_trie *>(t))->compress();
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
                t->add(v, version);
                break;
            case QUERY: {
                query q(v, version);
                queries.push(q);
                break;
            }
            case DELETION: {
                t->delete_ngram(v, version);
                break;
            }
            case FINISH: {
                /*
                 * TODO: Αν κάνουμε και τα add και delete παράλληλα χρειαζόμαστε ένα wait_all
                    tp.wait_all();
                 */
                auto *results = new string[queries.size()];


                for (int i = 0; i < (int) queries.size(); i++) {
                     tp.add_task(new search_task(t, queries.at(i), i, results));
                }
                // Print query results
                tp.wait_all();

//                for (int i = 0; i < queries.size(); i++) {
//                    cout << results[i] << endl; //                } 
                size_t k = 0;
                if (v.size() == 1) {
                    k = (size_t) helpers::to_int(v[0]);
                }

                if (!compress) {
                    trie_node *next_branch;

                    while ((next_branch = t->next_branch()) != nullptr) {
                        // Create tasks to clean up all branches
                        // tp.add_task(new clean_up_task(t, next_branch));
                         t->clean_up(next_branch);
                        /* Haven't tested it. Should work with lambdas as well */
                        // tp.add_task([t, next_branch] { t->clean_up(next_branch); });
                    }
                    t->reset_branch();
                }



                print_and_topk(results, (int) queries.size(), k);
                tp.wait_all();

                delete[] results;
//                version = 1;
                queries.clear(200);
                break;
            }
        default:
            break;
        }
        version++;
        v.clear(100);
        if (stop) break;
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
        else {
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
        for (int i = (int) max_freq-1; i >= 0 && counter != (int) topk; i--) {
            minHeap heap(array[i]);
            heap.heapSort();
            for (int j = 0; j < (int) array[i].size() && counter != (int) topk; j++) {
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
