#ifndef MY_HEADERS_LOGGER_H
#define MY_HEADERS_LOGGER_H

#include "date_time.hpp"
#include <iostream>
#include <fstream>
#include <string>

#define STDOUT 0
#define LOGFILE 1
#define BOTH 2
#define DEFAULT BOTH

namespace mstd {
    class logger {
    public:
        static std::string file;

        static void debug(const std::string &context, const std::string &message, int type = DEFAULT) {
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

        static void warn(const std::string &context, const std::string &message, int type = DEFAULT) {
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

        static void success(const std::string &context, const std::string &message, int type = DEFAULT) {
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


        static void error(const std::string &context, const std::string &message, int type = DEFAULT, bool stderr = true) {
            mstd::date_time now;
            if (type == STDOUT || type == BOTH) {
                std::ostream *outp;
                if (stderr) {
                    outp = &std::cerr;
                } else {
                    outp = &std::cout;
                }
                *outp << "\033[1;31m " << now() << " - [ERROR] \t-- " << context << " : "
                          << message << "\033[0m" << std::endl;
            }

            if (type == LOGFILE || type == BOTH) {
                std::ofstream outp(file.c_str(), std::ios::app);
                outp << now() << " - [ERROR] \t-- " << context << " : " << message << std::endl;
                outp.flush();
                outp.close();
            }
        }

        static void new_run() {
            std::ofstream outp(file.c_str(), std::ios::app);
            // There *must* be a better way -_-
            outp << "====================================================================================" << std::endl;
            outp.flush();
            outp.close();
        }
    };
}

#endif //MY_HEADERS_LOGGER_H
