#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <functional>
#include "task.hpp"
#include "worker.hpp"



class thread_pool
{
public:
    thread_pool(int num_threads);
    virtual ~thread_pool();

    void add_task(task *t);
    
    void add_task(std::function<void (void)> f);

    void finish();

    void wait_all();
private:
    std::vector<worker *> _threads;
    work_queue _wq;
    pthread_cond_t _finished_cond;
    pthread_mutex_t _finished_mtx;
    
    class _raw_task_ : public task {
    public:
        _raw_task_(std::function<void (void)> f);
        virtual void run() override;
    private:
        std::function<void (void)> _f;
        
    };
};

#endif // THREAD_POOL_H
