/*************************************************************************
	> File Name: Queue.h
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 16 Jul 2014 05:29:23 AM PDT
 ************************************************************************/

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stddef.h>

class Node{
	friend class Queue;
private:
	int data_;
	Node* next_;
};

typedef Node* NodePtr;

class Queue{
public:
	Queue();
    ~Queue();
	void push(int data);
	void pop();
	bool isEmpty() const;
	int top() const;
	size_t getSize() const;
	void clear();
private:
	NodePtr head_;
	NodePtr tail_;
	size_t size_;
};

#endif


