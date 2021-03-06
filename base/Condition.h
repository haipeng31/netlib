//Author: Zhan Haijing(haipeng31@gmail.com)

#ifndef CONDITION_H
#define CONDITION_H

#include "Mutex.h"
#include <pthread.h>

namespace mynet
{

class Condition
{
public:
	Condition(MutexLock& mutex) : mutex_(mutex)
	{
		pthread_cond_init(&pcond_, NULL);
	}

	~Condition()
	{
		pthread_cond_destroy(&pcond_);
	}

	void wait()
	{
		pthread_cond_wait(&pcond_, mutex_.getPthreadMutex());
	}

	void notify()
	{
		pthread_cond_signal(&pcond_); 
	}

	void notifyAll()
	{
		pthread_cond_broadcast(&pcond_);
	}

private:
	MutexLock& mutex_;
	pthread_cond_t pcond_;
};

}

#endif
