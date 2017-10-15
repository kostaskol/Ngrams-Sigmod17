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

        const char *c_str() const;

        char *c_str();

        std::string cpp_str();

        size_t length() const;

        vector<string> split(char *delim) const;

        vector<string> split(char delim) const;

        vector<string> split(const char *delim) const;

        string substr(int start, int length) const;

        int to_int() const;

        void remove(char c);

        void replace(char old, char n);

        bool contains(char *str) const;

        bool contains(const char *str) const;

        bool contains(const string &other) const;

        void remove_substr(const string &other);

        void remove_substr(const char *str);

        void remove_substr(char *str);

        bool starts_with(char *str) const;

        bool starts_with(const string &other) const;

        bool starts_with(const char *str) const;

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

        const bool operator==(const string &other) const;

        const bool operator==(const char *str) const;

        const bool operator!=(const string &other) const;

        const bool operator!=(const char *str) const;

        const bool operator>(const string &other) const;

        const bool operator>(const char *str) const;

        const bool operator<(const string &other) const;

        const bool operator<(const char *str) const;

        const char operator[](int index) const;

        const bool operator==(const string &other) ;

        const bool operator==(const char *str) ;

        const bool operator!=(const string &other) ;

        const bool operator!=(const char *str) ;

        const bool operator>(const string &other) ;

        const bool operator>(const char *str) ;

        const bool operator<(const string &other) ;

        const bool operator<(const char *str) ;

        const char operator[](int index) ;

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
