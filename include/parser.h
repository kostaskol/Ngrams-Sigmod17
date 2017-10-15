#ifndef TRIES_PARSER_H
#define TRIES_PARSER_H

#include "file_reader.h"

#define INSERTION 0
#define QUERY 1
#define DELETION 2
#define UNKNOWN_OP 3

class parser {
private:
    mstd::file_reader _file;

public:
    explicit parser(mstd::string file_name);

    ~parser() = default;

    bool is_open();

    bool next_init(mstd::vector<mstd::string> *);

    bool next_query(mstd::vector<mstd::string> *, int *);
};


#endif //TRIES_PARSER_H
