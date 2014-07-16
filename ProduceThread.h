/*************************************************************************
	> File Name: ProduceThread.h
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 16 Jul 2014 07:31:51 AM PDT
 ************************************************************************/

#ifndef PRODUCE_THREAD_H_
#define PRODUCE_THREAD_H_

#include <pthread.h>

class Buffer;

class ProduceThread{
public:
	ProduceThread(Buffer &buffer);

	void start();
	static void* threadFunc(void *arg);
	void run();
	void join();
private:
	pthread_t tid_;
	Buffer &buffer_;
};

#endif
