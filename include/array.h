#ifndef TRIES_ARRAY_H
#define TRIES_ARRAY_H

#include <cstdio>

namespace mstd {
    template <typename T>
    class array {
    private:
        T _arr[];
        size_t _size;
        size_t _used;
    public:
        array(size_t size);
        array(const array &);
        ~array();

        void set(int, T);
        T &get(int);

        void resize();
        void shrink();

        bool full();
        size_t size();
        size_t used();

        T &operator[](int);
        array &operator=(const array &);
    };
}


#endif //TRIES_ARRAY_H
