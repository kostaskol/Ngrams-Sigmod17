#include "date_time.hpp"
#include "constants.hpp"
#include "logger.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

bool mstd::logger::_run = false;
std::string mstd::logger::file = "../debug/log.txt";

void mstd::logger::debug(const std::string &context, const std::string &message, int type) {
    if (!_run) {
        _new_run();
        _run = true;
    }
    std::string outp_str = mstd::date_time::now()() + " - [DEBUG] \t-- "
                           + context + " : " + message;
    if (type == constants::STDOUT || type == constants::BOTH_STREAMS) {
        std::cout << outp_str << std::endl;
    }

    if (type == constants::LOGFILE || type == constants::BOTH_STREAMS) {
        std::ofstream outp(file.c_str(), std::ios::app);
        outp << outp_str << std::endl;
        outp.flush();
        outp.close();
    }
}

void mstd::logger::warn(const std::string &context, const std::string &message, int type) {
    if (!_run) {
        _new_run();
        _run = true;
    }
    mstd::date_time now;
    if (type == constants::STDOUT || type == constants::BOTH_STREAMS) {
        std::cout << "\033[1;33m " << now()
                  << " - [WARNING] \t-- " << context << " : " << message << "\033[0m" << std::endl;
    }

    if (type == constants::LOGFILE || type == constants::BOTH_STREAMS) {
        std::ofstream outp(file.c_str(), std::ios::app);
        outp << now.to_string()
             << " - [WARNING] \t-- " << context << " : " << message << std::endl;
        outp.flush();
        outp.close();
    }
}

void mstd::logger::success(const std::string &context, const std::string &message, int type) {
    if (!_run) {
        _new_run();
        _run = true;
    }
    mstd::date_time now;
    if (type == constants::STDOUT || type == constants::BOTH_STREAMS) {
        std::cout << "\033[1;32m " << now()
                  << " - [SUCCESS] \t-- " << context << " : " << message << "\033[0m" << std::endl;
    }

    if (type == constants::LOGFILE || type == constants::BOTH_STREAMS) {
        std::ofstream outp(file.c_str(), std::ios::app);
        outp << now()
             << " - [SUCCESS] \t-- " << context << " : " << message << std::endl;
        outp.flush();
        outp.close();
    }
}


void mstd::logger::error(const std::string &context, const std::string &message, int type, bool stderr) {
    if (!mstd::logger::_run) {
        _new_run();
        _run = true;
    }
    mstd::date_time now;
    if (type == constants::STDOUT || type == constants::BOTH_STREAMS) {
        std::ostream *outp;
        if (stderr) {
            outp = &std::cerr;
        } else {
            outp = &std::cout;
        }
        *outp << "\033[1;31m " << now() << " - [ERROR] \t-- " << context << " : "
              << message << "\033[0m" << std::endl;
    }

    if (type == constants::LOGFILE || type == constants::BOTH_STREAMS) {
        std::ofstream outp(file.c_str(), std::ios::app);
        outp << now() << " - [ERROR] \t-- " << context << " : " << message << std::endl;
        outp.flush();
        outp.close();
    }
}

void mstd::logger::_new_run() {
    _mkdir();
    std::ofstream outp(file.c_str(), std::ios::app);
    // There *must* be a better way to make new runs obvious -_-
    outp << "====================================================================================" << std::endl;
    outp.flush();
    outp.close();
}

void mstd::logger::_mkdir() {
    int status = mkdir("../debug", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}