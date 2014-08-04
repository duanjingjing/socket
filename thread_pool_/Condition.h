/*************************************************************************
	> File Name: Condition.h
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Sun 03 Aug 2014 08:37:12 PM PDT
 ************************************************************************/

#ifndef CONDITION_H
#define CONDITION_H

#include <pthread.h>
#include "NonCopyable.h"

class MutexLock;

class Condition:NonCopyable{
public:
	Condition(MutexLock &mutex);
	~Condition();

	void wait();
	void notify();
	void notifyAll();
private:
	pthread_cond_t cond_;
    MutexLock &mutex_;
};

#endif
