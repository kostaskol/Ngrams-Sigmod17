#ifndef TRIES_PARSER_H
#define TRIES_PARSER_H

#include <string>
#include <fstream>
#include "mvector.hpp"

#define INSERTION 0
#define QUERY 1
#define DELETION 2
#define FINISH 3
#define UNKNOWN_OP 4

class parser {
private:
    std::ifstream _file;

public:
    explicit parser(std::string file_name);

    ~parser() = default;

    bool is_open();

    bool next_init(mstd::vector<std::string> *);

    bool next_command(mstd::vector<std::string> *, int *);
};


#endif //TRIES_PARSER_H
