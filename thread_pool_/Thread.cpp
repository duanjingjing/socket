/*************************************************************************
	> File Name: Thread.cpp
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Sun 03 Aug 2014 11:55:15 PM PDT
 ************************************************************************/

#include "Thread.h"

Thread::Thread(ThreadFunc callback):tid_(0),
				isStarted_(false),
				callback_(callback)
{

}

Thread::~Thread(){
	if(isStarted_){
		pthread_detach(tid_);
	}
}

void* Thread::threadFunc(void *arg){
	Thread *pt=static_cast<Thread*>(arg);
	pt->callback_();//调用线程逻辑
	return NULL;
}

void Thread::start(){
	pthread_create(&tid_,NULL,threadFunc,this);
}

void Thread::join(){
	pthread_join(tid_,NULL);
}


