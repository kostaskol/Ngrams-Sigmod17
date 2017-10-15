#ifndef MYSTRING_H_
#define MYSTRING_H_

#include <iostream>
#include "mvector.h"

namespace mstd {

// Simple string class
// that includes some of the usual std::string operations
    class string {
    private:
        char *_str;
        size_t _len;
    public:
        string();

        string(char *str);

        string(const char *str);

        string(const string &other);

        string(char c);

        string(const int num);

        string rstrip();

        virtual ~string();

        const char *c_str();

        std::string cpp_str();

        size_t length();

        vector<string> split(char *delim);

        vector<string> split(char delim);

        vector<string> split(const char *delim);

        string substr(int start, int length);

        int to_int();

        void remove(char c);

        void replace(char old, char n);

        bool contains(char *str);

        bool contains(const char *str);

        bool contains(const string &other);

        void remove_substr(const string &other);

        void remove_substr(const char *str);

        void remove_substr(char *str);

        bool starts_with(char *str);

        bool starts_with(const string &other);

        bool starts_with(const char *str);

        string &operator=(const string &other);

        string &operator=(const char *str);

        string &operator=(char *str);

        string &operator+=(const string &other);

        string &operator+=(const char *str);

        string &operator+=(const char c);

        string &operator+=(int num);

        friend string operator+(const string &first, const string &other);

        friend string operator+(const string &first, const char *second);

        friend string operator+(const char *first, const string &other);

        const bool operator==(const string &other);

        const bool operator==(const char *str);

        const bool operator!=(const string &other);

        const bool operator!=(const char *str);

        const bool operator>(const string &other);

        const bool operator>(const char *str);

        const bool operator<(const string &other);

        const bool operator<(const char *str);

        const char operator[](int index);

        friend std::ostream &operator<<(std::ostream &out, mstd::string str);
    };
}

namespace mstd {
    std::ostream &operator<<(std::ostream &out, mstd::string str);

    string operator+(const string &first, const string &other);

    string operator+(const string &first, const char *second);

    string operator+(const char *first, const string &other);
}
#endif /* MYSTRING_H_ */
