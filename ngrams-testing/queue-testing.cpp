#include <stdexcept>
#include "mvector.hpp"
// #include <string.h>
#include "mqueue.hpp"
#include <gtest/gtest.h>

using mstd::vector;
using mstd::queue;

class QueueTest_Max : public ::testing::Test {
public:
    queue<int> q;
    QueueTest_Max() : q(5) {}   //queue with max size 5 entities
};

class QueueTest_NoMax : public ::testing::Test {
public:
    queue<int> q;
    virtual void SetUp(){
        q;
    }
};

TEST_F(QueueTest_NoMax, sizeTest){
    for (size_t i = 0; i < 100; i++) {
        q.push(i);
    }
    EXPECT_EQ(q.size(), 100);
}

TEST_F(QueueTest_NoMax, pushAtEmpty){
    int x = 1;
    q.push(x);
    EXPECT_EQ(q.size(), 1);
    EXPECT_EQ(q.peek(), x);
}

TEST_F(QueueTest_NoMax, pushAtNonEmpty){
    for (size_t i = 0; i < 5; i++) {
        q.push(i);
    }
    for (size_t i = 0; i < 5; i++) {
        EXPECT_EQ(q.get_element_at(i), i);
    }
}

TEST_F(QueueTest_NoMax, popAtEmpty){
    ASSERT_THROW(q.pop(),std::runtime_error);
}

TEST_F(QueueTest_NoMax, popAtNonEmpty){
    q.push(1); q.push(2);

    EXPECT_EQ(q.pop(), 1) << "Popped wrong item";
    ASSERT_EQ(q.size(), 1);
}

TEST_F(QueueTest_NoMax, getElementAtPosition){
    for (size_t i = 0; i < 5; i++) {
        q.push(i);
    }
    ASSERT_NO_THROW(q.get_element_at(2));
    ASSERT_EQ(q.get_element_at(2), 2);
}

TEST_F(QueueTest_NoMax, emptyTest){
    ASSERT_TRUE(q.empty());
}

TEST_F(QueueTest_NoMax, clearTest){
    q.push(1);
    q.push(2);
    q.pop(); q.pop();
    q.clear();
    ASSERT_EQ(q.size(), 0);
    ASSERT_EQ(q.get_head(), nullptr);
    ASSERT_EQ(q.get_last(), nullptr);
}

TEST_F(QueueTest_NoMax, peekTest){
    q.push(5); q.push(6);
    ASSERT_NO_THROW(q.peek());
    ASSERT_EQ(q.peek(), 5);
}
 /*---------- Tests for queue with a max size ----------*/
TEST_F(QueueTest_Max, fullTest){
    for (size_t i = 0; i < 5; i++) {
        q.push(i);
    }
    ASSERT_THROW(q.push(5), std::runtime_error);
}
