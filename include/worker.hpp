#ifndef WORKER_H
#define WORKER_H

#include "work_queue.hpp"
#include "task.hpp"
#include "thread.hpp"

class worker : public thread
{
public:
    worker(work_queue &, 
    		pthread_mutex_t *, 
    		pthread_cond_t *,
			int *);
protected:
    virtual void run() override;
private:
    work_queue &_wq;
    pthread_mutex_t *_finished_mtx;
    pthread_cond_t *_finished_cond;
	int *_num_finished;
};

#endif // WORKER_H
