#include "work_queue.hpp"
#include <iostream>
using namespace std;
work_queue::work_queue() {
    pthread_mutex_init(&q_mtx, nullptr);
    pthread_cond_init(&q_cond, nullptr);
}

work_queue::~work_queue() {
    pthread_mutex_destroy(&q_mtx);
    pthread_cond_destroy(&q_cond);
}

task *work_queue::next_task() {
    task *t;
    pthread_mutex_lock(&q_mtx);
    while (tasks.empty()) {
        pthread_cond_wait(&q_cond, &q_mtx);
    }

    t = tasks.pop();

    pthread_mutex_unlock(&q_mtx);

    return t;
}

void work_queue::add_task(task *t) {
    pthread_mutex_lock(&q_mtx);

    tasks.push(t);

    pthread_cond_signal(&q_cond);

    pthread_mutex_unlock(&q_mtx);
}

int work_queue::size() {
    pthread_mutex_lock(&q_mtx);

    auto size = (int) tasks.size();

    pthread_mutex_unlock(&q_mtx);

    return size;
}