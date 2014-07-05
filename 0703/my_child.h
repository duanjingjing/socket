/*************************************************************************
	> File Name: my_child.h
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Thu 03 Jul 2014 04:28:02 AM PDT
 ************************************************************************/

#ifndef _MY_CHILD_H
#define _MY_CHILD_H
#include"my_socket.h"

#define S_BUSY 1
#define S_IDLE 0

typedef struct tag{
    pid_t s_pid;
	int   s_status;
	int   s_read;
	int   s_write;
	int   s_cnt;
}CHLD,*pCHLD;
void make_chld(pCHLD arr,int cnt);
void child_main();

#endif
