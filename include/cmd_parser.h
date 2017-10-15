#ifndef TRIES_CMD_PARSER_H
#define TRIES_CMD_PARSER_H


#include "mstring.h"
#include "hash_table.h"

#define UNKNOWN_ARG 1
#define WRONG_FORMAT 2
#define WRONG_INPUT 3
#define SUCCESS 4


/*
 * Simple command line arguments parser. Expects an mstd::hash_table object
 * with entries in the form of: "argument-name" => "<s/i/none>"
 * and parses the command line arguments trying to match them
 */

namespace mstd {
    class cmd_parser {
    private:
        mstd::hash_table<mstd::string> _args;
        mstd::vector<mstd::string> _flags;
    public:
        cmd_parser() : _args(5), _flags(3) {}

        ~cmd_parser() = default;

        int parse(int argc, char **argv, mstd::hash_table<mstd::string> &ht, bool allow_unknown = false) {
            for (int i = 1; i < argc; i++) {
                mstd::string s = argv[i];
                mstd::string type;
                bool known = true;
                try {
                    type = ht.get(s);
                } catch (std::runtime_error &e) {
                    known = false;
                }

                if (!known && !allow_unknown) {
                    std::cout << "Unknown key: " << s << std::endl;
                    return UNKNOWN_ARG;
                }

                if (type == "<none>") {
                    _flags.add(s);
                } else if (type == "<i>") {
                    if (i + 1 >= argc) {
                        return WRONG_FORMAT;
                    } else {
                        _args.put(s, argv[++i]);
                    }
                } else if (type == "<s>") {
                    if (i + 1 >= argc) {
                        return WRONG_FORMAT;
                    } else {
                        _args.put(s, argv[++i]);
                    }
                } else {
                    return WRONG_INPUT;
                }
            }
            _flags.shrink_to_size();
            return SUCCESS;
        }

        mstd::string get_string(const mstd::string &key) const {
            return _args.get(key);
        }

        int get_int(const mstd::string &key) {
            return atoi(_args.get(key).c_str());
        }

        bool is_set(const mstd::string &key) {
            return _flags.in(key);
        }
    };
}
#endif //TRIES_CMD_PARSER_H
