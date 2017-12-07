#ifndef BIT_VECTOR
#define BIT_VECTOR
#include <cstddef>

//using byte = unsigned char;
typedef unsigned char byte;
class bit_vector {
private:
    byte *_entries;
    size_t _size;

    bool _test(byte b, byte offset);
    bool _set(byte &b, byte offset);
public:
    // Size is the number of *bits* that
    // the bit array will hold
    explicit bit_vector(size_t size);
    bit_vector(const bit_vector &);
    ~bit_vector();

    void set(size_t index);

    bool check(size_t index);

    bool check_and_set(size_t index);

    bit_vector &operator=(const bit_vector &);
};

#endif