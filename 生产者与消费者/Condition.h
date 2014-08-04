/*************************************************************************
	> File Name: Condition.h
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 16 Jul 2014 05:54:23 AM PDT
 ************************************************************************/

#ifndef _CONDITION_H_
#define _CONDITION_H_

#include <pthread.h>

class MutexLock;

class Condition{
public:
	Condition(MutexLock &lock);
	~Condition();
	void wait();
	void notify();
	void notifyAll();
private:
	pthread_cond_t cond_;
	MutexLock &lock_;
};

#endif
