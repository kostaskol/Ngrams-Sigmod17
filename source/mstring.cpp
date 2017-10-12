#include <cstring>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include "mstring.h"

using namespace std;

mstd::string::string() : _len(1) {
    _str = new char[1];
    _str[0] = '\0';
    _len = 1;
}

mstd::string::string(char *str) {

    _len = (int) strlen(str) + 1;
    _str = new char[_len];
    strcpy(_str, str);
}

mstd::string::string(const char *str) {
    _len = (int) strlen(str) + 1;
    _str = new char[_len];
    strcpy(_str, str);
}

mstd::string::string(char c) {
    _str = new char[2];
    _len = 2;
    _str[0] = (char) c;
    _str[1] = '\0';
}

mstd::string::string(const mstd::string &other) {
    if (other._str != nullptr) {
        _len = (int) strlen(other._str) + 1;
        _str = new char[_len + 1];
        strcpy(_str, other._str);
        return;
    }

    _len = 1;
    _str = new char[1];
    _str[0] = '\0';
}

mstd::string::string(int num) {
    char tmp[20];
    sprintf(tmp, "%d", num);

    _str = new char[strlen(tmp) + 1];
    strcpy(_str, tmp);
    _len = strlen(_str) + 1;
}


mstd::string::~string() {
    delete[] _str;
    _str = nullptr;
}

const char *mstd::string::c_str() {
    return _str;
}

std::string mstd::string::cpp_str() {
	std::string str = _str;
	return str;
}

size_t mstd::string::length() { return strlen(_str); }

mstd::vector<mstd::string> mstd::string::split(char *delim) {

    mstd::vector<mstd::string> list;

    char *tmp_str = new char[strlen(_str) + 1];
    strcpy(tmp_str, _str);
    char *token = strtok(tmp_str, delim);

    while (token != nullptr) {
        list.push(token);
        token = strtok(nullptr, delim);
    }

    delete[] tmp_str;
    return list;
}

mstd::vector<mstd::string> mstd::string::split(const char *delim) {
	mstd::vector<mstd::string> list;

    char *tmp_str = new char[strlen(_str) + 1];
    strcpy(tmp_str, _str);
    char *token = strtok(tmp_str, delim);

    while (token != nullptr) {
        list.push(token);
        token = strtok(NULL, delim);
    }

    delete[] tmp_str;
    return list;
}

mstd::vector<mstd::string> mstd::string::split(char delim) {
	mstd::vector<mstd::string> list;
	
	mstd::string tmp_str = "";
	bool pushed = false;
	for (int c = 0; c < (int) strlen(_str); c++) {
		if (_str[c] == delim) {
			list.push(tmp_str);
			pushed = true;
			tmp_str = "";
		} else {
			tmp_str += _str[c];
			pushed = false;
		}
	}
	
	if (!pushed) {
		list.push(tmp_str);
	}
	
	return list;
}

mstd::string mstd::string::substr(int start, int length) {
    if ((start + length) >= (int) _len) {
        throw std::runtime_error("Bad length");
    }

    char *tmp = new char[length + 2];

    int j = 0;
    for (int i = start; i <= start + length; i++) {
        tmp[j++] = _str[i];
    }
    tmp[j] = '\0';

    mstd::string tmp_str = tmp;
    delete[] tmp;
    return tmp_str;

}

void mstd::string::replace(char old, char n) {
    for (int i = 0; i < (int) strlen(_str); i++) {
        if (_str[i] == old)
            _str[i] = n;
    }
}

int mstd::string::to_int() { return atoi(_str); }

void mstd::string::remove(char c) {
    char *tmp = new char[strlen(_str) + 1];

    int j = 0;
    for (size_t i = 0; i < strlen(_str) + 1; i++) {
        if (_str[i] == c) continue;
        tmp[j++] = _str[i];
    }

    delete[] _str;
    _str = tmp;
    _len = strlen(tmp) + 1;
}

bool mstd::string::contains(char *str) {
    return strstr(_str, str) != nullptr;
}

bool mstd::string::contains(const mstd::string& other) {
    return strstr(_str, other._str) != nullptr;
}

bool mstd::string::contains(const char *str) {
    return strstr(_str, str) != nullptr;
}

void mstd::string::remove_substr(const mstd::string &other) {
    remove_substr(other._str);
}

void mstd::string::remove_substr(const char *str) {
    size_t length = strlen(str);
    size_t this_len = strlen(_str);
    char *sub = strstr(_str, str);
    if (sub == nullptr) return;
    char *tmp = new char[strlen(_str) - length + 1];
    char *tmp_str = _str;
    char *tmp_tmp = tmp;
    for (size_t i = 0; i < strlen(_str) + 1; i++) {
        if (tmp_str != sub) {
            *tmp_tmp = *tmp_str;
            tmp_str++;
            tmp_tmp++;
        } else {
            i += length;
            tmp_str += length;
        }
    }

    delete[] _str;
    _str = tmp;
    _str[this_len - length] = '\0';
    _len = strlen(_str) + 1;
}

void mstd::string::remove_substr(char *str) {
    size_t length = strlen(str);
    size_t this_len = strlen(_str);
    char *sub = strstr(_str, str);
    if (sub == nullptr) return;
    char *tmp = new char[strlen(_str) - length + 1];
    char *tmp_str = _str;
    char *tmp_tmp = tmp;
    for (size_t i = 0; i < strlen(_str) + 1; i++) {
        if (tmp_str != sub) {
            *tmp_tmp = *tmp_str;
            tmp_str++;
            tmp_tmp++;
        } else {
            i += length;
            tmp_str += length;
        }
    }

    delete[] _str;
    _str = tmp;
    _str[this_len - length] = '\0';
    _len = strlen(_str) + 1;
}

bool mstd::string::starts_with(char *str) {
    char *c = strstr(_str, str);
    return c == _str;
}

bool mstd::string::starts_with(const mstd::string &other) {
    return starts_with(other._str);
}

bool mstd::string::starts_with(const char *str) {
    char *c = strstr(_str, str);
    // If _str contains str
    // AND the start of that mstring is the start of the mstring (_str)
    // we return true
    return c == _str;
}

mstd::string &mstd::string::operator=(const mstd::string& other) {
    delete[] _str;

    if (other._str == nullptr) {
        _len = 0;
        _str = new char[1];
        _str[0] = '\0';
        return *this;
    }

    _len = (int) strlen(other._str) + 1;
    _str = new char[_len + 1];
    strcpy(_str, other._str);
    return *this;
}

mstd::string &mstd::string::operator=(const char *str) {
    delete[] _str;

    _len = strlen(str) + 1;
    _str = new char[_len];
    strcpy(_str, str);
    return *this;
}

mstd::string &mstd::string::operator=(char *str) {
    delete[] _str;
    if (str == nullptr) {
        _len = 0;
        _str = new char[1];
        _str[0] = '\0';
        return *this;
    }

    _len = strlen(str) + 1;
    _str = new char[_len];
    strcpy(_str, str);
    return *this;
}

mstd::string &mstd::string::operator+=(const mstd::string& other) {

    char *tmp = new char[_len + other._len + 2];

    strcpy(tmp, _str);

    delete[] _str;

    strcat(tmp, other._str);


    _str = new char[_len + other._len + 2];

    strcpy(_str, tmp);

    delete[] tmp;

    _len += other._len - 1;


    return *this;
}



mstd::string &mstd::string::operator+=(const char *str) {
    char *tmp = new char[strlen(_str) + strlen(str) + 2];


    strncpy(tmp, _str, strlen(_str) + 1);
    delete[] _str;
    strcat(tmp, str);

    _str = new char[strlen(tmp) + 1];

    strcpy(_str, tmp);

    delete[] tmp;

    _len += strlen(_str) + 1;

    return *this;
}


mstd::string &mstd::string::operator+=(const char c) {
    _len += 1;

    char *tmp = new char[_len + 1];

    strcpy(tmp, _str);

    delete[] _str;

    _str = tmp;

    _str[_len - 2] = (char) c;
    _str[_len - 1] = '\0';

    return *this;
}

mstd::string &mstd::string::operator+=(int num) {
    mstd::string tmp(num);

    char *tmp_str = new char[_len + 1];
    strcpy(tmp_str, _str);
    delete[] _str;

    _str = new char[_len + tmp._len + 2];
    strcpy(_str, tmp_str);
    strcat(_str, tmp._str);

    delete[] tmp_str;

    _len = strlen(_str) + 1;

    return *this;
}

const char mstd::string::operator[](int index) {
    if (index > (int) (_len - 1)) {
        throw std::runtime_error("Index out of range");
    }

    return _str[index];
}

mstd::string mstd::operator+(const mstd::string &first, const mstd::string &second) {
    mstd::string tmp = first;
    return tmp += second;
}

mstd::string mstd::operator+(const mstd::string &first, const char *other) {
    mstd::string tmp = first;
    return tmp += other;
}

mstd::string mstd::operator+(const char *first, const mstd::string &other) {
    mstd::string tmp = first;
    return tmp += other;
}

const bool mstd::string::operator==(const mstd::string& other) {
    if (_str == nullptr) return false;
    return (strcmp(_str, other._str) == 0);
}

const bool mstd::string::operator==(const char *str) {
    if (str == nullptr) return false;
    if (_str == nullptr) return false;
    return (strcmp(_str, str) == 0);
}

const bool mstd::string::operator!=(const mstd::string &other) {
    if (_str == nullptr) return false;
    return (strcmp(_str, other._str) != 0);
}

const bool mstd::string::operator!=(const char *str) {
    if (_str == nullptr) return false;
    return (strcmp(_str, str) != 0);
}

const bool mstd::string::operator>(const mstd::string &other) {
    return strcmp(_str, other._str) > 0;
}

const bool mstd::string::operator>(const char *str) {
    return strcmp(_str, str) > 0;
}

const bool mstd::string::operator<(const mstd::string &other) {
    return strcmp(_str, other._str) < 0;
}

const bool mstd::string::operator<(const char *str) {
    return strcmp(_str, str) < 0;
}


std::ostream &mstd::operator<<(std::ostream &out, mstd::string str) {
    if (str._str == nullptr) {
        out << "null";
    } else {
        out << str._str;
    }
    return out;
}