#ifndef MY_HEADERS_LOGGER_H
#define MY_HEADERS_LOGGER_H

#include "mstring.h"

#include <iostream>
#include <fstream>

namespace mstd {
    class logger {
    public:
        static void debug(const mstd::string &context, const mstd::string &message) {
            std::cout << "[DEBUG] :: " << context << " : " << message << std::endl;
        }

        static void debug(const mstd::string &&context, const mstd::string &&message) {
            debug(context, message);
        }

        static void f_debug(const mstd::string &context, const mstd::string &message, mstd::string &file) {
            std::ofstream outp(file.c_str(), std::ios::app);
            outp << "[DEBUG] \t:: " << context << " : " << message << std::endl;
            outp.flush();
            outp.close();
        }

        static void f_debug(const mstd::string &&context, const mstd::string &&message, mstd::string &&file) {
            f_debug(context, message, file);
        }

        static void warn(const mstd::string &context, const mstd::string &message) {
            std::cout << "\033[1;33m [WARNING] :: " << context << " : " << message << "\033[0m" << std::endl;
        }

        static void warn(const mstd::string &&context, const mstd::string &&message) {
            warn(context, message);
        }

        static void f_warn(const mstd::string &context, const mstd::string &message, mstd::string &file) {
            std::ofstream outp(file.c_str(), std::ios::app);
            outp << "[WARNING] \t:: " << context << " : " << message << std::endl;
            outp.flush();
            outp.close();
        }

        static void f_warn(const mstd::string &&context, const mstd::string &&message, mstd::string &&file) {
            f_warn(context, message, file);
        }

        static void success(const mstd::string &context, const mstd::string &message) {
            std::cout << "\033[1;32m [SUCCESS] :: " << context << " : " << message << "\033[0m" << std::endl;
        }

        static void success(const mstd::string &&context, const mstd::string &&message) {
            success(context, message);
        }

        static void f_success(const mstd::string &context, const mstd::string &message, mstd::string &file) {
            std::ofstream outp(file.c_str(), std::ios::app);
            outp << "[SUCCESS] \t:: " << context << " : " << message << std::endl;
            outp.flush();
            outp.close();
        }

        static void f_success(const mstd::string &&context, const mstd::string &&message, mstd::string &&file) {
            f_success(context, message, file);
        }

        static void error(const mstd::string &context, const mstd::string &message) {
            std::cerr << "\033[1;31m[ERROR] :: " << context << " : " << message << "\033[0m" << std::endl;
        }

        static void error(const mstd::string &&context, const mstd::string &&message) {
            error(context, message);
        }

        static void f_error(const mstd::string &context, const mstd::string &message, mstd::string &file) {
            std::ofstream outp(file.c_str(), std::ios::app);
            outp << "[ERROR] \t:: " << context << " : " << message << std::endl;
            outp.flush();
            outp.close();
        }

        static void f_error(const mstd::string &&context, const mstd::string &&message, mstd::string &&file) {
            f_error(context, message, file);
        }
    };
}

#endif //MY_HEADERS_LOGGER_H
