#include "thread_pool.hpp"
#include <iostream>
#include <cmath>

using namespace std;

thread_pool::thread_pool(int num_threads) {
    pthread_cond_init(&_finished_cond, nullptr);
    pthread_mutex_init(&_finished_mtx, nullptr);

    for (int i = 0; i < num_threads; i++) {
        _threads.push(new worker(_wq, &_finished_mtx, &_finished_cond));
        _threads.back()->start();
    }

}

thread_pool::~thread_pool() {
    pthread_mutex_destroy(&_finished_mtx);
    pthread_cond_destroy(&_finished_cond);

    finish();
}

void thread_pool::add_task(task *t) {
    _wq.add_task(t);
}

void thread_pool::add_task(std::function<void (void)> f) {
    task *t = new _raw_task_(f);
    _wq.add_task(t);
}

void thread_pool::finish() {
    size_t size = _threads.size();
    for (size_t i = 0; i < size; i++) {
        // Inserting null tasks kills the thread that tries to run it
        _wq.add_task(nullptr);
    }

    size = _threads.size();
    for (size_t i = 0; i < size; i++) {
        _threads[i]->join();
        delete _threads[i];
    }

    _threads.clear();
}


void thread_pool::wait_all() {
    if (_wq.size() == 0) {
        return;
    }
    // The thread that executes the last job in the queue,
    // will also signal us that we're done with everything
    pthread_mutex_lock(&_finished_mtx);
    pthread_cond_wait(&_finished_cond, &_finished_mtx);
    pthread_mutex_unlock(&_finished_mtx);
}



/* -- Raw Task -- */

thread_pool::_raw_task_::_raw_task_(std::function<void ()> f) : _f(f) { }

void thread_pool::_raw_task_::run() {
    _f();
    
    delete this;
}



