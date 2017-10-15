#include <fcntl.h>
#include "file_reader.h"
#include <unistd.h>

using namespace std;

mstd::file_reader::file_reader(mstd::string filename) {
    _fd = open(filename.c_str(), O_RDONLY);
    _is_open = _fd > -1;
}

mstd::file_reader::~file_reader() {
    if (_is_open) {
        close(_fd);
    }
}

// Returns true on EOF
bool mstd::file_reader::next_line(mstd::string *line) {
    char *buf = new char[1024];
    *line = "";
    bool has_read = false;
    do {
        ssize_t bytes = read(_fd, buf, 1023);
        if (bytes != 0) {
            buf[bytes] = '\0';
            for (int i = 0; i < bytes; i++) {
                *line += buf[i];
                has_read = true;
                if (buf[i] == '\n') {
                    lseek(_fd, i - bytes + 1, SEEK_CUR);
                    delete[] buf;
                    //return !(bytes != 1024 && bytes != i);
                    return false;
                }
            }
        } else if (has_read) {
            return false;
        } else {
            delete[] buf;
            return true;
        }
    } while (true);
}

bool mstd::file_reader::is_open() {
    return _is_open;
}

void mstd::file_reader::close_file() {
    if (_is_open) {
        close(_fd);
        _is_open = false;
    }
}
