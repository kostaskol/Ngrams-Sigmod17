#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

class thread {
    public:
        thread();
        virtual ~thread();

        void start();

        void join();

        pthread_t get_tid();
        bool get_joined();

    protected:
        virtual void run() = 0;
    private:
        static void *_t_func(void *arg);

        enum T_State {
            T_State_None,
            T_State_Started,
            T_State_Joined
        };

        T_State state;
        pthread_t tid;
        bool joined;
};

#endif // THREAD_H
