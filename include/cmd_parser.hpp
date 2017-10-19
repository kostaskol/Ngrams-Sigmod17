#ifndef TRIES_CMD_PARSER_H
#define TRIES_CMD_PARSER_H


#include <string>
#include "hash_table.hpp"
#include "helpers.hpp"

#define UNKNOWN_ARG 1
#define WRONG_FORMAT 2
#define WRONG_INPUT 3
#define SUCCESS 4


/*
 * Simple command line arguments parser. Expects an mstd::hash_table object
 * with entries in the form of: "argument-name" => "<u(nary)/b(inary)>"
 * and parses the command line arguments trying to match them
 */

namespace mstd {
    class cmd_parser {
    private:
        mstd::hash_table<std::string> _args;
        mstd::vector<std::string> _flags;
        bool _auto_msg;
        std::string _legal;
        void _exit(const std::string &&exec_name, int err_type) {
            switch (err_type) {
                case UNKNOWN_ARG:
                    std::cout << "Unknown arguments found. Usage: " << exec_name << _legal << std::endl;
                    exit(-1);
                case WRONG_FORMAT:
                    std::cout << "Wrong usage found. Usage: " << exec_name << _legal << std::endl;
                    exit(-1);
                case WRONG_INPUT:
                    std::cerr << "Our input was wrong" << std::endl;
                    exit(-1);
            }
        }
    public:
        explicit cmd_parser(bool auto_msg = false, std::string &&legal = "")
                : _args(5), _flags(3), _auto_msg(auto_msg), _legal(legal) {}

        ~cmd_parser() = default;

        int parse(int argc, char **argv, mstd::hash_table<std::string> &ht, bool allow_unknown = false) {
            for (int i = 1; i < argc; i++) {
                std::string s = argv[i];
                std::string type;
                bool known = true;
                try {
                    type = ht.get(s);
                } catch (std::runtime_error &e) {
                    known = false;
                }

                if (!known && !allow_unknown) {
                    if (_auto_msg) {
                        _exit(std::string(argv[0]), UNKNOWN_ARG);
                    }
                    std::cout << "Unknown key: " << s << std::endl;
                    return UNKNOWN_ARG;
                }

                if (type == "<u>") {
                    _flags.add(s);
                } else if (type == "<b>") {
                    if (i + 1 >= argc) {
                        if (_auto_msg) {
                            _exit(std::string(argv[0]), WRONG_FORMAT);
                        }
                        return WRONG_FORMAT;
                    } else {
                        _args.put(s, argv[++i]);
                    }
                } else {
                    if (_auto_msg) {
                        _exit(std::string(argv[0]), WRONG_INPUT);
                    }
                    return WRONG_INPUT;
                }
            }
            _flags.shrink_to_size();
            return SUCCESS;
        }

        std::string get_string(const std::string &key) const {
            return _args.get(key);
        }

        int get_int(const std::string &key) {
            return helpers::to_int(_args.get(key));
        }

        bool is_set(const std::string &key) {
            return _flags.in(key);
        }
    };
}
#endif //TRIES_CMD_PARSER_H
