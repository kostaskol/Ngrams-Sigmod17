#ifndef CSV_PARSER_FILE_READER_H
#define CSV_PARSER_FILE_READER_H


#include "mstring.h"

namespace mstd {

    class file_reader {
    private:
        int _fd;
        mstd::string _line;
        bool _is_open;
    public:
        explicit file_reader(mstd::string filename);

        ~file_reader();

        bool next_line(mstd::string *);

        bool is_open();

        void close_file();
    };

}


#endif //CSV_PARSER_FILE_READER_H
