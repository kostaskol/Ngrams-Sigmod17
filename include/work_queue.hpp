#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H

#include "mqueue.hpp"
#include <pthread.h>
#include "task.hpp"

class work_queue {
    public:
        work_queue();
        virtual ~work_queue();

        void add_task(task *t);

        task *next_task();

        int size();
    private:
        mstd::queue<task *> tasks;
        pthread_mutex_t q_mtx;
        pthread_cond_t q_cond;
};

#endif // WORK_QUEUE_H
