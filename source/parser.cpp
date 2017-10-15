#include "parser.h"
#include <string>
#include <sstream>

using std::string;
using mstd::vector;
using std::cout;
using std::endl;

parser::parser(string file_name) : _file(file_name) {}

bool parser::next_init(vector<string> *line) {
    string s;
    std::getline(_file, s);
    const auto str_end = s.find_last_not_of('\n');
    s.substr(0, str_end);
    std::istringstream ss(s);
    string tmp;
    while (getline(ss, s, ' ')) {
        line->add(s);
    }

    return _file.eof();
}

bool parser::next_query(vector<std::string> *line, int *type) {
    string s;
    std::getline(_file, s);
    const auto str_end = s.find_last_not_of('\n');
    s.substr(0, str_end);
    std::istringstream ss(s);
    string tmp;
    vector<string> tmp_v;
    while(getline(ss, s, ' ')) {
        tmp_v.add(s);
    }

    if (tmp_v[0] == "A") {
        *type = INSERTION;
    } else if (tmp_v[0] == "Q") {
        *type = QUERY;
    } else if (tmp_v[0] == "D") {
        *type = DELETION;
    } else {
        *type = UNKNOWN_OP;
    }

    *line = vector<string>(tmp_v, 1, (int) tmp.size());

    return _file.eof();
}

bool parser::is_open() {
    return _file.is_open();
}