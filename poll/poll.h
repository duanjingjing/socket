/*************************************************************************
	> File Name: poll.h
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Thu 31 Jul 2014 04:47:24 AM PDT
 ************************************************************************/

#ifndef POLL_H_
#define POLL_H_

#include <poll.h>

#define ERR_EXIT(m) \
	do{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

typedef struct{
    struct pollfd clients_[2048];
	int listenfd_;
	int maxi_;
	int nready_;
	void (*handle_callback_)(int,char*);//回调函数
}poll_t;

void poll_init(poll_t *pol,int listenfd,void (*handler)(int,char*));
void poll_do_wait(poll_t *pol);
void poll_handle_accept(poll_t *pol);
void poll_handle_data(poll_t *pol);

#endif
