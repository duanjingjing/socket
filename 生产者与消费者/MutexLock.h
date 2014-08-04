/*************************************************************************
	> File Name: MutexLock.h
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 16 Jul 2014 05:45:12 AM PDT
 ************************************************************************/

#ifndef MUTEX_LOCK_
#define MUTEX_LOCK_

#include <pthread.h>
#include <stdexcept>
class MutexLock{
public:
	MutexLock(){
		if(pthread_mutex_init(&lock_,NULL)){
			throw std::runtime_error("init failed!");
		}
	}

	~MutexLock(){
       if(pthread_mutex_destroy(&lock_)){
		   throw std::runtime_error("destroy failed");
	   }
	}

	void lock(){
		if(pthread_mutex_lock(&lock_)){
			throw std::runtime_error("lock failed!");
		}
	}

	void unlock(){
		if(pthread_mutex_unlock(&lock_)){
			throw std::runtime_error("unlock failed!");
		}
	}

    pthread_mutex_t *getMutexLockPtr(){
		return &lock_;
	}
private:
	pthread_mutex_t lock_;
};

#endif
