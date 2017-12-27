#ifndef TRIES_CMD_PARSER_H
#define TRIES_CMD_PARSER_H

#include <string>
#include <iostream>
#include <getopt.h>

namespace cmd_parser {
    struct cmd_args {
        std::string init_file;
        std::string query_file;
        int num_threads;

        cmd_args() : init_file(""), query_file(""), num_threads(-1) { }

        cmd_args &operator=(const cmd_args &other)=default;
    };

    void print_help(const char *name) {
        std::cout << "Printing help for " << name << "\n"
                  << "[Required Arguments]\n"
                  << "\t--init|-i\t: The trie initialisation file\n"
                  << "\t--query|-q\t: The trie work file\n"
                  << "[Optional Arguments]\n"
                  << "\t--threads|-t\t: The number of threads to be run in the thread pool (Default 4)\n"
                  << "\t--help|-h\t: Prints this message and exits the program" << std::endl;
    }

    cmd_args parse(int argc, char **argv) {
        const std::string short_opts = "i:q:t:";
        const option long_opts[] = {
                {"init", required_argument, nullptr, 'i'},
                {"query", required_argument, nullptr, 'q'},
                {"threads", required_argument, nullptr, 't'},
                {"help", no_argument, nullptr, 'h'}
        };

        cmd_args args;

        int c,
            index;
        while ((c = getopt_long(argc, argv, short_opts.c_str(), long_opts, &index)) != -1) {
            switch (c) {
                case 'i':
                    args.init_file = optarg;
                    break;
                case 'q':
                    args.query_file = optarg;
                    break;
                case 't': {
                    char *endptr;
                    args.num_threads = (int) strtol(optarg, &endptr, 10);
                    break;
                }
                case 'h':
                case '?':
                    print_help(argv[0]);
                    exit(-1);
            }
        }
        return args;
    }
}

#endif //TRIES_CMD_PARSER_H
