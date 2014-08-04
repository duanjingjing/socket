/*************************************************************************
	> File Name: Queue.cpp
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 16 Jul 2014 05:33:02 AM PDT
 ************************************************************************/

#include "Queue.h"
#include <assert.h>

Queue::Queue():head_(NULL),tail_(NULL),size_(0){
}

Queue::~Queue(){
	clear();
}

void Queue::push(int data){
	NodePtr pt=new Node;
	pt->data_=data;
	if(isEmpty()){
		head_=tail_=pt;
	}
	else{
		tail_->next_=pt;
		tail_=pt;
	}
	size_++;
}

void Queue::pop(){
	assert(!isEmpty());
    NodePtr pt=head_;
	head_=head_->next_;
	delete pt;
	size_--;
}

int Queue::top() const{
	assert(!isEmpty());
	return head_->data_;
}

void Queue::clear(){
	while(isEmpty()){
		pop();
	}
}

bool Queue::isEmpty() const{
	return head_==NULL;
}

size_t Queue::getSize() const{
	return size_;
}




