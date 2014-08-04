/*************************************************************************
	> File Name: ProduceThread.cpp
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 16 Jul 2014 07:34:18 AM PDT
 ************************************************************************/
#include "ProduceThread.h"
#include "Buffer.h"
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

ProduceThread::ProduceThread(Buffer &buffer):tid_(0),buffer_(buffer){
	::srand(2525);
}

void ProduceThread::start(){
	pthread_create(&tid_,NULL,threadFunc,this);
}

void* ProduceThread::threadFunc(void* arg){
	ProduceThread *pt=static_cast<ProduceThread*>(arg);
	pt->run();
	return NULL;
}

void ProduceThread::run(){
	while(1){
		int data=rand()%1000;
		cout<<"produce a data: "<<data<<endl;
		buffer_.produce(data);
		sleep(5);
	}
}

void ProduceThread::join(){
	pthread_join(tid_,NULL);
}
