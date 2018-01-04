#include <string>
#include <sstream>
#include "mvector.hpp"

namespace helpers {
    inline void split(const std::string &s, mstd::vector<std::string> &v, char delim) {
        std::stringstream ss(s);
        std::string tmp;
        while(std::getline(ss, tmp, delim)) {
            if (tmp != "") {
                v.m_push(tmp);
            }
        }
    }

    inline std::string join(const mstd::vector<std::string> &v, char on) {
        std::string s;
        for (int i = 0; i < (int) v.size() - 1; i++) {
            s += v[i] + on;
        }
        s += v[v.size() - 1];
        return s;
    }

    inline int to_int(const std::string &s) {
        std::stringstream ss(s);
        int x = 0;
        ss >> x;
        return x;
    }
}
