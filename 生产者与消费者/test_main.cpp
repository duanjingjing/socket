/*************************************************************************
	> File Name: test_main.cpp
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 16 Jul 2014 07:53:19 AM PDT
 ************************************************************************/
#include "Buffer.h"
#include "ConsumeThread.h"
#include "ProduceThread.h"
#include<iostream>
using namespace std;

int main(int argc,char *argv[]){
	Buffer buffer(5);
	ProduceThread p1(buffer);
	ProduceThread p2(buffer);
	ProduceThread p3(buffer);

	ConsumeThread c1(buffer);
	ConsumeThread c2(buffer);
	ConsumeThread c3(buffer);

	p1.start();
	p2.start();
	p3.start();

	c1.start();
	c2.start();
	c3.start();

	p1.join();
	p2.join();
	p3.join();

	c1.join();
	c2.join();
	c3.join();

	return 0;
}

