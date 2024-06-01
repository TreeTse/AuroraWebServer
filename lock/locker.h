#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <pthread.h>
#include <semaphore.h>


class Sem
{
public:
    Sem() {
        if (sem_init(&sem_, 0, 0) != 0) {
            throw std::exception();
        }
    }
    Sem(int num) {
        if (sem_init(&sem_, 0, num) != 0) {
            throw std::exception();
        }
    }
    ~Sem() {
        sem_destroy(&sem_);
    }
    bool Wait() {
        return sem_wait(&sem_) == 0;
    }
    bool Post() {
        return sem_post(&sem_) == 0;
    }

private:
    sem_t sem_;
};

class Locker
{
public:
    Locker()
    {
        if (pthread_mutex_init(&mutex_, NULL) != 0) {
            throw std::exception();
        }
    }
    ~Locker()
    {
        pthread_mutex_destroy(&mutex_);
    }
    bool Lock()
    {
        return pthread_mutex_lock(&mutex_) == 0;
    }
    bool Unlock()
    {
        return pthread_mutex_unlock(&mutex_) == 0;
    }
    pthread_mutex_t *Get()
    {
        return &mutex_;
    }

private:
    pthread_mutex_t mutex_;
};

class Cond
{
public:
    Cond()
    {
        if (pthread_cond_init(&cond_, NULL) != 0) {
            throw std::exception();
        }
    }
    ~Cond()
    {
        pthread_cond_destroy(&cond_);
    }
    bool Wait(pthread_mutex_t *mutex_)
    {
        int ret = 0;
        ret = pthread_cond_wait(&cond_, mutex_);
        return ret == 0;
    }
    bool Timewait(pthread_mutex_t *mutex_, struct timespec t)
    {
        int ret = 0;
        ret = pthread_cond_timedwait(&cond_, mutex_, &t);
        return ret == 0;
    }
    bool Signal()
    {
        return pthread_cond_signal(&cond_) == 0;
    }
    bool Broadcast()
    {
        return pthread_cond_broadcast(&cond_) == 0;
    }

private:
    pthread_cond_t cond_;
};
#endif
