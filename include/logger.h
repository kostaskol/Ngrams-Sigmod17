#ifndef MY_HEADERS_LOGGER_H
#define MY_HEADERS_LOGGER_H

#include "mstring.h"

#include <iostream>
#include <fstream>

#define STDOUT 0
#define LOGFILE 1
#define BOTH 2

namespace mstd {
    class logger {
    public:
        static mstd::string file;

        static void debug(const mstd::string &context, const mstd::string &message, int type) {
            if (type == STDOUT || type == BOTH) {
                std::cout << "[DEBUG] :: " << context << " : " << message << std::endl;
            }

            if (type == LOGFILE || type == BOTH) {
                std::ofstream outp(file.c_str(), std::ios::app);
                outp << "[DEBUG] \t:: " << context << " : " << message << std::endl;
                outp.flush();
                outp.close();
            }
        }

        static void warn(const mstd::string &context, const mstd::string &message, int type) {
            if (type == STDOUT || type == BOTH) {
                std::cout << "\033[1;33m [WARNING] :: " << context << " : " << message << "\033[0m" << std::endl;
            }

            if (type == LOGFILE || type == BOTH) {
                std::ofstream outp(file.c_str(), std::ios::app);
                outp << "[WARNING] \t:: " << context << " : " << message << std::endl;
                outp.flush();
                outp.close();
            }
        }

        static void success(const mstd::string &context, const mstd::string &message, int type) {
            if (type == STDOUT || type == BOTH) {
                std::cout << "\033[1;32m [SUCCESS] :: " << context << " : " << message << "\033[0m" << std::endl;
            }

            if (type == LOGFILE || type == BOTH) {
                std::ofstream outp(file.c_str(), std::ios::app);
                outp << "[SUCCESS] \t:: " << context << " : " << message << std::endl;
                outp.flush();
                outp.close();
            }
        }


        static void error(const mstd::string &context, const mstd::string &message, int type) {
            if (type == STDOUT || type == BOTH) {
                std::cerr << "\033[1;31m[ERROR] :: " << context << " : " << message << "\033[0m" << std::endl;
            }

            if (type == STDOUT || type == BOTH) {
                std::ofstream outp(file.c_str(), std::ios::app);
                outp << "[ERROR] \t:: " << context << " : " << message << std::endl;
                outp.flush();
                outp.close();
            }
        }
    };
}

#endif //MY_HEADERS_LOGGER_H
