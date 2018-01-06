#include <cmath>

inline bool is_prime(int val) {
    bool f = true;
    if (val % 2 == 0) return false;

    for (int i = 3, root = (int) sqrt(val); i < root; i += 2) {
        if (val % i == 0) {
            f = false;
            break;
        }
    }
    return f;
}
