/*************************************************************************
	> File Name: Condition.cpp
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Sun 03 Aug 2014 11:48:26 PM PDT
 ************************************************************************/

#include "Condition.h"
#include "MutexLock.h"

Condition::Condition(MutexLock &mutex):mutex_(mutex){
	pthread_cond_init(&cond_,NULL);
}

Condition::~Condition(){
	pthread_cond_destroy(&cond_);
}

void Condition::wait(){
	//wait前必须lock
	assert(mutex_.isLocked());
	pthread_cond_wait(&cond_,mutex_.getMutexPtr());
}

void Condition::notify(){
	pthread_cond_signal(&cond_);
}

void Condition::notifyAll(){
	pthread_cond_broadcast(&cond_);
}


