/*************************************************************************
	> File Name: ConsumeThread.h
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 16 Jul 2014 07:40:13 AM PDT
 ************************************************************************/

#ifndef CONSUME_THREAD_H_
#define CONSUME_THREAD_H_

#include <pthread.h>
class Buffer;

class ConsumeThread{
public:
	ConsumeThread(Buffer &buffer);

	void start();
	static void* threadFunc(void *arg);
	void run();
	void join();
private:
	pthread_t tid_;
	Buffer &buffer_;
};

#endif
