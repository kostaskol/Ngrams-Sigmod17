#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "mvector.hpp"
#include <functional>
#include "task.hpp"
#include "worker.hpp"



class thread_pool
{
public:
    explicit thread_pool(int num_threads);
    virtual ~thread_pool();

    void add_task(task *t);
    
    void add_task(std::function<void (void)> f);

    void finish();

    void wait_all();

    int get_active();
private:
    mstd::vector<worker *> _threads;
    work_queue _wq;
    pthread_mutex_t _finished_mtx;
    pthread_cond_t _finished_cond;

    int _num_finished;
    int _num_assigned;
    
    class _raw_task_ : public task {
    public:
        explicit _raw_task_(std::function<void (void)> f);
        void run() override;
    private:
        std::function<void (void)> _f;
    };

};

#endif // THREAD_POOL_H
