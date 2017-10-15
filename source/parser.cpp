#include "parser.h"

using mstd::string;
using mstd::vector;
using std::cout;
using std::endl;

parser::parser(string file_name) : _file(file_name) {}

bool parser::next_init(vector<string> *line) {
    string s;
    bool eof = _file.next_line(&s);
    s.rstrip();
    *line = s.split(' ');

    return eof;
}

bool parser::next_query(vector<mstd::string> *line, int *type) {
    string s;
    bool eof = _file.next_line(&s);
    s.rstrip();
    vector<string> tmp = s.split(' ');
    if (tmp[0] == "A") {
        *type = INSERTION;
    } else if (tmp[0] == "Q") {
        *type = QUERY;
    } else if (tmp[0] == "D") {
        *type = DELETION;
    } else {
        *type = UNKNOWN_OP;
    }
    *line = vector<string>(tmp, 1, (int) tmp.size());
    return eof;
}

bool parser::is_open() {
    return _file.is_open();
}