#ifndef TRIES_DATE_TIME_H
#define TRIES_DATE_TIME_H

#include <ctime>
#include <string>

#define DATE 0
#define TIME 1
#define BOTH 2

namespace mstd {

    class date_time {
    public:
        int d, m, y, H, M, S;

        date_time() {
            time_t ms;
            time(&ms);
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

        std::string to_string(const int type = BOTH, const std::string &sep = ":") {
            switch (type) {
                case DATE:
                    return get_date_as_string();
                case TIME:
                    return get_time_as_string(sep);
                case BOTH:
                    return get_date_time_as_string(sep);
                default:
                    return "Unknown type";
            }
        }

        std::string operator()() {
            return to_string();
        }
    private:
        std::string get_date_as_string() {
            return std::string(d < 10 ? "0" : "") + std::to_string(d) + "/"
                   + std::string(m < 10 ? "0" : "") + std::to_string(m) + "/"
                   + std::to_string(y);
        }

        std::string get_time_as_string(const std::string &sep = ":") {
            return std::string(H < 10 ? "0" : "") + std::to_string(H) + sep
                   + std::string(M < 10 ? "0" : "") + std::to_string(M) + sep
                   + std::string(S < 10 ? "0" : "") + std::to_string(S);
        }

        std::string get_date_time_as_string(const std::string &sep = ":") {
            return get_date_as_string() + " - " + get_time_as_string(sep);
        }


    };
}
#endif //TRIES_DATE_TIME_H
