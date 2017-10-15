#ifndef MY_HEADERS_LOGGER_H
#define MY_HEADERS_LOGGER_H

#include <string>
#include "date_time.h"

#include <iostream>
#include <fstream>

#define STDOUT 0
#define LOGFILE 1
#define BOTH 2

namespace mstd {
    class logger {
    public:
        static std::string file;

        static void debug(const std::string &context, const std::string &message, int type = BOTH) {
            std::string outp_str = mstd::date_time::now()() + " - [DEBUG] \t-- "
                                    + context + " : " + message;
            if (type == STDOUT || type == BOTH) {
                std::cout << outp_str << std::endl;
            }

            if (type == LOGFILE || type == BOTH) {
                std::ofstream outp(file.c_str(), std::ios::app);
                outp << outp_str << std::endl;
                outp.flush();
                outp.close();
            }
        }

        static void warn(const std::string &context, const std::string &message, int type = BOTH) {
            mstd::date_time now;
            if (type == STDOUT || type == BOTH) {
                std::cout << "\033[1;33m " << now()
                          << " - [WARNING] \t-- " << context << " : " << message << "\033[0m" << std::endl;
            }

            if (type == LOGFILE || type == BOTH) {
                std::ofstream outp(file.c_str(), std::ios::app);
                outp << now.to_string()
                     << " - [WARNING] \t-- " << context << " : " << message << std::endl;
                outp.flush();
                outp.close();
            }
        }

        static void success(const std::string &context, const std::string &message, int type = BOTH) {
            mstd::date_time now;
            if (type == STDOUT || type == BOTH) {
                std::cout << "\033[1;32m " << now()
                          << " - [SUCCESS] \t-- " << context << " : " << message << "\033[0m" << std::endl;
            }

            if (type == LOGFILE || type == BOTH) {
                std::ofstream outp(file.c_str(), std::ios::app);
                outp << now()
                     << " - [SUCCESS] \t-- " << context << " : " << message << std::endl;
                outp.flush();
                outp.close();
            }
        }


        static void error(const std::string &context, const std::string &message, int type = BOTH) {
            mstd::date_time now;
            if (type == STDOUT || type == BOTH) {
                std::cerr << "\033[1;31m " << now() << " - [ERROR] \t-- " << context << " : "
                          << message << "\033[0m" << std::endl;
            }

            if (type == STDOUT || type == BOTH) {
                std::ofstream outp(file.c_str(), std::ios::app);
                outp << now() << " - [ERROR] \t-- " << context << " : " << message << std::endl;
                outp.flush();
                outp.close();
            }
        }
    };
}

#endif //MY_HEADERS_LOGGER_H
