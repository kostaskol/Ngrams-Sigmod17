#include "worker.hpp"

worker::worker(work_queue &wq, 
				pthread_mutex_t *finished_mtx, 
				pthread_cond_t *finished_cond) : _wq(wq), 
												_finished_mtx(finished_mtx),
												_finished_cond(finished_cond) { }

void worker::run() {
    // If we add a nullptr task to the work queue, the thread will die
    while (task *t = _wq.next_task()) {
        t->run();
        if (_wq.size() == 0) {
        	pthread_mutex_lock(_finished_mtx);
        	pthread_cond_signal(_finished_cond);
        	pthread_mutex_unlock(_finished_mtx);
    	}
    }
}
