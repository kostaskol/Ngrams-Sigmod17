#include <gtest/gtest.h>
#include "is_prime.hpp"
#include "task.hpp"
#include "thread_pool.hpp"

class is_prime_task : public task {
public:
    is_prime_task(int val) : _val(val) { }

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

class ThreadPoolTest : public ::testing::Test {
public:
    thread_pool tp;
    ThreadPoolTest() : tp(4) { }
};

TEST_F(ThreadPoolTest, fewTasksTest) {
    task **tasks = new task*[3];
    int tests[] = { 12, 37, 181 };

    for (int i = 0; i < 3; i++) {
        tasks[i] = new is_prime_task(tests[i]);
    }

    for (int i = 0; i < 3; i++) {
        tp.add_task(tasks[i]);
    }

    tp.wait_all();

    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(dynamic_cast<is_prime_task *>(tasks[i])->get_result(), is_prime(tests[i]));
    }

    for (int i = 0; i < 3; i++) {
        delete tasks[i];
    }

    delete[] tasks;
    tp.finish();
}

TEST_F(ThreadPoolTest, aLotOfTasksTest) {
    int tests[] = { 10, 12, 15, 23, 31, 21, 82, 112, 1101, 1021 };
    task **tasks = new task*[10];

    for (int i = 0; i < 10; i++) {
        tasks[i] = new is_prime_task(tests[i]);
        tp.add_task(tasks[i]);
    }

    tp.wait_all();
    
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(dynamic_cast<is_prime_task *>(tasks[i])->get_result(), is_prime(tests[i]));
        delete tasks[i];
    }

    delete[] tasks;
    tp.finish();
}
