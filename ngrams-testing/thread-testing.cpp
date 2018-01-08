#include <gtest/gtest.h>
#include <cmath>
#include "thread.hpp"
#include "is_prime.hpp"

class is_prime_thread : public thread {
public:
    is_prime_thread() : _val(0), _result(true) { }

    void set_val(int val) {
        _val = val;
    }

    bool get_result() {
        return _result;
    }

    void run() override {
        _result = is_prime(_val);
    }

private:
    int _val;
    bool _result;
};

class PrimeTest : public ::testing::Test {
public:
    is_prime_thread t;
};

TEST_F(PrimeTest, smallNumberTest) {
    t.set_val(5);
    t.start();
    t.join();
    EXPECT_EQ(t.get_result(), true);
}

TEST_F(PrimeTest, mediumNumberTest) {
    t.set_val(1187);
    t.start();
    t.join();
    EXPECT_EQ(t.get_result(), true);
}

TEST_F(PrimeTest, largeNumberTest) {
    t.set_val(11189);
    t.start();
    t.join();
    EXPECT_EQ(t.get_result(), false);
}
