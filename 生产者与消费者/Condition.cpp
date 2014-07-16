/*************************************************************************
	> File Name: Condition.cpp
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 16 Jul 2014 05:58:28 AM PDT
 ************************************************************************/

#include "Condition.h"
#include "MutexLock.h"
#include <stdexcept>

using namespace std;

Condition::Condition(MutexLock &lock):lock_(lock){
	if(pthread_cond_init(&cond_,NULL)){
		throw std::runtime_error("init cond error!");
	}
}

Condition::~Condition(){
	if(pthread_cond_destroy(&cond_)){
		throw std::runtime_error("destroy cond error");
	}
}

void Condition::wait(){
	if(pthread_cond_wait(&cond_,lock_.getMutexLockPtr())){
		throw std::runtime_error("wait cond error");
	}
}

void Condition::notify(){
	if(pthread_cond_signal(&cond_)){
		throw std::runtime_error("signal error");
	}
}

void Condition::notifyAll(){
	if(pthread_cond_broadcast(&cond_)){
		throw std::runtime_error("broadcast error");
	}
}



