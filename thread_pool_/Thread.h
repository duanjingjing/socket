/*************************************************************************
	> File Name: Thread.h
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Sun 03 Aug 2014 11:52:40 PM PDT
 ************************************************************************/

#ifndef THREAD_H
#define THREAD_H

#include "NonCopyable.h"
#include <pthread.h>
#include <functional>

class Thread:private NonCopyable{
public:
	typedef std::function<void ()> ThreadFunc;

	Thread(ThreadFunc callback);
	~Thread();

	void start();
	void join();
private:

	static void* threadFunc(void*);

	pthread_t tid_;
	bool isStarted_;
	ThreadFunc callback_;
};



#endif
