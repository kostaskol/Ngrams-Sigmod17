#include "parser.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <helpers.hpp>

using std::string;
using mstd::vector;
using std::cout;
using std::endl;

parser::parser(string file_name) : _file(file_name) {}

bool parser::next_init(vector<string> *line) {
    if (!_file.is_open()) throw std::runtime_error("File not opened");
    if (_file.eof()) throw std::runtime_error("Already reached eof");

    string s;
    std::getline(_file, s);
    const auto str_end = s.find_last_not_of('\n');
    s.substr(0, str_end);
    helpers::split(s, *line, ' ');

    return _file.eof();
}

bool parser::next_command(vector<string> *line, int *type) {
    if (!_file.is_open()) throw std::runtime_error("File not opened");
    if (_file.eof()) throw std::runtime_error("Already reached eof");
    string s;
    std::getline(_file, s);
    const auto str_end = s.find_last_not_of('\n');
    s.substr(0, str_end);
    vector<string> tmp_v;

    helpers::split(s, tmp_v, ' ');
    if (tmp_v.size() != 0) {
        if (tmp_v[0] == "A") {
            *type = INSERTION;
        } else if (tmp_v[0] == "Q") {
            *type = QUERY;
        } else if (tmp_v[0] == "D") {
            *type = DELETION;
        } else if (tmp_v[0] == "F") {
            *type = FINISH;
        } else {
            *type = UNKNOWN_OP;
        }

        for (int i = 1; i < tmp_v.size(); i++) {
            line->m_push(tmp_v[i]);
        }

        return _file.eof();
    }
    return true;
}

bool parser::is_open() {
    return _file.is_open();
}
