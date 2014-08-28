/*************************************************************************
	> File Name: session.h
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 27 Aug 2014 09:04:34 PM PDT
 ************************************************************************/

#ifndef SESSION_H_
#define SESSION_H_

#include "common.h"

typedef struct{
	char command[1024];
	char comm[1024];
	char args[1024];
	int peerfd;//客户连接的fd

	int nobody_fd;//nobody所使用的fd
	int proto_fd;//proto进程使用的fd
}session_t;


//初始化session
void session_init(session* sess);

//
void session_begin(session* sess);



#endif
