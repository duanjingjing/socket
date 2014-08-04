/*************************************************************************
	> File Name: Buffer.h
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 16 Jul 2014 05:06:05 AM PDT
 ************************************************************************/

#ifndef BUFFER_H_
#define BUFFER_H_

#include "MutexLock.h"
#include "Condition.h"
#include "Queue.h"

class Buffer{
public:
	Buffer(size_t size);
	~Buffer();
	bool isEmpty() const;
	bool isFull() const;

	void produce(int data);
	int consume();
private:
	Queue queue_;
    mutable MutexLock mutex_;
	Condition full_;
	Condition empty_;
	size_t size_;
};

#endif
