#ifndef MY_HEADERS_LOGGER_H
#define MY_HEADERS_LOGGER_H

#include "constants.hpp"
#include <string>

// TODO: Solve the error message when using a function from mstd::logger
namespace mstd {
    class logger {
    public:
        static std::string file;

        static void debug(const std::string &context, const std::string &message, int type = DEFAULT_STREAM);
        
        static void warn(const std::string &context, const std::string &message, int type = DEFAULT_STREAM);

        static void success(const std::string &context, const std::string &message, int type = DEFAULT_STREAM);

        static void error(const std::string &context, const std::string &message, int type = DEFAULT_STREAM, bool stderr = true);

    private:
        static bool _run;
        
        static void new_run();
    };
}


#endif //MY_HEADERS_LOGGER_H
