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
private:
    mstd::vector<worker *> _threads;
    work_queue _wq;
    pthread_mutex_t _finished_mtx;
    pthread_cond_t _finished_cond;
//    pthread_mutex_t _wait_all_mtx;
//    pthread_cond_t _wait_all_cond;

    int _num_finished;
    int _num_assigned;
//    bool _die;
    
    class _raw_task_ : public task {
    public:
        explicit _raw_task_(std::function<void (void)> f);
        void run() override;
    private:
        std::function<void (void)> _f;
    };

//    class __notifier : public thread {
//    public:
//        __notifier()
//                : _finished_mtx(nullptr),
//                  _finished_cond(nullptr),
//                  _wait_all_mtx(nullptr),
//                  _wait_all_cond(nullptr),
//                  _finished(nullptr),
//                  _die(nullptr) { }
//
//        void initialise(pthread_mutex_t *finished_mtx,
//                        pthread_cond_t *finished_cond,
//                        pthread_mutex_t *wait_all_mtx,
//                        pthread_cond_t *wait_all_cond,
//                        bool *finished,
//                        bool *die) {
//
//            _finished_mtx = finished_mtx;
//            _finished_cond = finished_cond;
//
//            _wait_all_mtx = wait_all_mtx;
//            _wait_all_cond = wait_all_cond;
//
//            _finished = finished;
//            _die = die;
//        }
//
//        void run() override {
//            while (true) {
//                pthread_mutex_lock(_finished_mtx);
//                pthread_cond_wait(_finished_cond, _finished_mtx);
//
//                pthread_mutex_lock(_wait_all_mtx);
//                if (*_die) {
//                    pthread_mutex_unlock(_finished_mtx);
//                    pthread_mutex_unlock(_wait_all_mtx);
//                    break;
//                }
//                *_finished = true;
//                pthread_cond_signal(_wait_all_cond);
//                pthread_mutex_unlock(_wait_all_mtx);
//            }
//        }
//    private:
//        pthread_mutex_t *_finished_mtx;
//        pthread_cond_t *_finished_cond;
//        pthread_mutex_t *_wait_all_mtx;
//        pthread_cond_t *_wait_all_cond;
//        bool *_finished;
//        bool *_die;
//    };
};

#endif // THREAD_POOL_H
