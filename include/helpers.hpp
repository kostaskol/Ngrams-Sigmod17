#ifndef TRIES_HELPERS_H
#define TRIES_HELPERS_H

#include <string>
#include <sstream>
#include "mvector.hpp"


namespace helpers {
    void split(const std::string &s, mstd::vector<std::string> &v, char delim) {
        std::stringstream ss(s);
        std::string tmp;
        while(std::getline(ss, tmp, delim)) {
            v.add(tmp);
        }
    }

    std::string join(const mstd::vector<std::string> &v, char on) {
        std::string s;
        for (int i = 0; i < v.size() - 1; i++) {
            s += v[i] + on;
        }
        s += v[v.size() - 1];
        return s;
    }
}

#endif //TRIES_HELPERS_H
