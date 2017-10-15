#ifndef TRIES_DATE_TIME_H
#define TRIES_DATE_TIME_H

#include <ctime>
#include <mstring.h>

class date_time {
public:
    int d, m, y, H, M, S;

    date_time() {
        time_t ms;
        struct tm *info = localtime(&ms);
        d = info->tm_mday;
        m = info->tm_mon + 1;
        y = info->tm_year + 1900;

        H = info->tm_hour;
        M = info->tm_min;
        S = info->tm_sec;
    }

    date_time(int d, int m, int y, int H, int M, int S) : d(d), m(m), y(y), H(H), M(M), S(S) {}

    date_time(int d, int m, int y) : d(d), m(m), y(y), H(0), M(0), S(0) {}

    static date_time now() {
        return {}; // Return an object initialised with the default constructor
    }

    mstd::string get_date_as_string() {
        return mstd::string(d < 10 ? "0" : "") + d + "/"
                + mstd::string(m < 10 ? "0" : "") + m + "/"
                + y;
    }

    mstd::string get_time_as_string(const mstd::string &sep = ":") {
        return mstd::string(H < 10 ? "0" : "") + H + sep
                + mstd::string(M < 10 ? "0" : "") + M + sep
                + mstd::string(S < 10 ? "0" : "") + S;
    }

    mstd::string get_date_time_as_string(const mstd::string &sep = ":") {
        return get_date_as_string() + " - " + get_time_as_string(sep);
    }
};

#endif //TRIES_DATE_TIME_H
