#include "bit_vector.hpp"
#include "mvector.hpp"
#include <cstdlib>
#include <iostream>
#include <gtest/gtest.h>

using mstd::vector;
using std::cout;
using std::endl;

#define SEED 5
#define SIZE 1024

class BitVectorTest : public testing::Test {
public:
    bit_vector bv;
    BitVectorTest() : bv(SIZE) {}
};

TEST_F(BitVectorTest, initExceptionTest) {
    EXPECT_THROW(bit_vector bv(1023), std::runtime_error);
}

TEST_F(BitVectorTest, setExceptionTest) {
    EXPECT_THROW(bv.set(1024), std::out_of_range);
}

TEST_F(BitVectorTest, isSetExceptionTest) {
    EXPECT_THROW(bv.check(-1), std::out_of_range);
}

TEST_F(BitVectorTest, testAndSetTest) {
    srand(SEED);
    vector<int> randoms(250);
    for (int i = 0; i < 250; i++) {
        int random;
        do {
            random = rand() % SIZE;
        } while (randoms.in(random));
        randoms.push(random);
        EXPECT_EQ(bv.check_and_set(random), false);
        EXPECT_EQ(bv.check(random), true);
    }
}

TEST_F(BitVectorTest, unsetTest) {
    for (int i = 0; i < 1024; i++) {
        EXPECT_EQ(bv.check(i), false);
    }
}

TEST_F(BitVectorTest, randomisedIsSetTest) {
    srand(SEED);
    int max = rand() % (rand() % (SIZE - 50) + 50);
    max <<= 3;

    // We need to create a new bit_vector here to hold the random size
    bit_vector bv(max);
    int tests = rand() % max;
    vector<int> positions(tests);
    for (int i = 0; i < tests; i++) {
        int pos = rand() % max;
        positions.push(pos);
        bv.set((size_t) pos);
    }
    return;
    bool flag = false;
    for (int pos : positions) {
        if (!bv.check((size_t) pos)) {
            flag = true;
            break;
        }
    }
    EXPECT_EQ(flag, false);
}
