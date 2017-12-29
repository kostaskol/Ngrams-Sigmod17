#include "thread_pool.hpp"
#include "logger.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using mstd::logger;

thread_pool::thread_pool(int num_threads) : _num_finished(0), _num_assigned(0) {
    pthread_mutex_init(&_finished_mtx, nullptr);
    pthread_cond_init(&_finished_cond, nullptr);

    for (int i = 0; i < num_threads; i++) {
        _threads.push(new worker(_wq,
                                 &_finished_mtx,
                                 &_finished_cond,
                                 &_num_finished));
        _threads.back()->start();
    }

}

thread_pool::~thread_pool() {
    pthread_mutex_destroy(&_finished_mtx);
    pthread_cond_destroy(&_finished_cond);

    finish();
}

void thread_pool::add_task(task *t) {
    _num_assigned++;
    _wq.add_task(t);
}

void thread_pool::add_task(std::function<void (void)> f) {
    _num_assigned++;
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
    pthread_mutex_lock(&_finished_mtx);
    if (_num_finished == _num_assigned) {
        pthread_mutex_unlock(&_finished_mtx);
    } else {
        while (_num_finished != _num_assigned) {
            pthread_cond_wait(&_finished_cond, &_finished_mtx);
        }

        _num_finished = 0;
        pthread_mutex_unlock(&_finished_mtx);

        _num_assigned = 0;
    }
}

int thread_pool::get_active() {
    pthread_mutex_lock(&_finished_mtx);
    int num_threads = _num_finished;
    pthread_mutex_unlock(&_finished_mtx);

    return _num_assigned - num_threads;
}



/* -- Raw Task -- */

thread_pool::_raw_task_::_raw_task_(std::function<void ()> f) : _f(std::move(f)) { }

void thread_pool::_raw_task_::run() {
    _f();
    
    delete this;
}



