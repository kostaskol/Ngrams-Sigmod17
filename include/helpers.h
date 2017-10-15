#ifndef TRIES_HELPERS_H
#define TRIES_HELPERS_H

#include <string>
#include <sstream>
#include "mvector.h"


namespace helpers {
    void split(const std::string &s, mstd::vector<std::string> &v, char delim) {
        std::stringstream ss(s);
        std::string tmp;
        while(std::getline(ss, tmp, delim)) {
            v.add(tmp);
        }
    }
}

#endif //TRIES_HELPERS_H
