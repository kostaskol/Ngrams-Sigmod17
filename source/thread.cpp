#include "thread.hpp"
#include <iostream>

using std::cout;
using std::endl;
using std::cerr;

thread::thread() : joined(false) {
    state = T_State_None;
}

thread::~thread() {
    if (state != T_State_Joined) {
        cerr << "Destroyed non-joined thread" << endl;
    }
}

bool thread::get_joined() {
    return joined;
}

pthread_t thread::get_tid() {
    return tid;
}

void thread::start() {
    if (state != T_State_None) {
        throw std::runtime_error("Starting already started or joined thread");
    }
    joined = false;
    pthread_create(&tid, nullptr, _t_func, this);
    state = T_State_Started;
}

void thread::join() {
    if (state != T_State_Started) {
        throw std::runtime_error("Joining non-started or already joined thread");
    }
    pthread_join(tid, nullptr);
    joined = true;
    state = T_State_Joined;
}

void *thread::_t_func(void *arg) {
    thread *t = static_cast<thread *>(arg);
    t->run();
    return nullptr;
}
