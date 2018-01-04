#include <stdexcept>
#include "mvector.hpp"
#include "mstack.hpp"
#include <gtest/gtest.h>

using mstd::vector;
using mstd::stack;

class StackTest_Max : public ::testing::Test {
public:
    stack<int> s;
    StackTest_Max() : s(5) {} //stack with max size 5 entities
};

class StackTest_NoMax : public ::testing::Test {
public:
    stack<int> s;
    virtual void SetUp() {
        s;
    }
};

TEST_F(StackTest_NoMax, sizeTest) {
    for (size_t i = 0; i < 100; i++) {
        s.push((int)i);
    }
    EXPECT_EQ(s.size(), 100);
}

TEST_F(StackTest_NoMax, pushAtEmpty) {
    int x = 1;
    s.push(1);
    EXPECT_EQ(s.size(), 1);
    EXPECT_EQ(s.peek(), x);
}

TEST_F(StackTest_NoMax, pushAtNonEmpty) {
    for (size_t i = 0; i < 5; i++) {
        s.push((int)i);
    }
    for (size_t i = 0; i < 5; i++) {
        EXPECT_EQ(s.get_element_at(i), 4-i);
    }
}

TEST_F(StackTest_NoMax, popAtEmpty) {
    ASSERT_THROW(s.pop(), std::runtime_error);
}

TEST_F(StackTest_NoMax, popAtNonEmpty) {
    s.push(1); s.push(2);

    EXPECT_EQ(s.pop(), 2);
    EXPECT_EQ(s.size(), 1);
}

TEST_F(StackTest_NoMax, getElementAtPosition) {
    for (size_t i = 0; i < 5; i++) {
        s.push((int)i);
    }
    ASSERT_NO_THROW(s.get_element_at(2));
    ASSERT_EQ(s.get_element_at(2), 2);
}

TEST_F(StackTest_NoMax, emptyTest) {
    ASSERT_TRUE(s.empty());
}

TEST_F(StackTest_NoMax, peekTest){
    s.push(5); s.push(6);
    ASSERT_NO_THROW(s.peek());
    ASSERT_EQ(s.peek(), 6);
}

TEST_F(StackTest_NoMax, clearTest) {
    s.push(1);
    s.push(2);
    s.clear();
    ASSERT_EQ(s.size(), 0);
    ASSERT_THROW(s.peek(), std::runtime_error);
    ASSERT_ANY_THROW(s.get_element_at(1));
}

TEST_F(StackTest_Max, fullTest){
    for (size_t i = 0; i < 5; i++) {
        s.push(i);
    }
    ASSERT_THROW(s.push(5), std::runtime_error);
}


