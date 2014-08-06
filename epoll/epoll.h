/*************************************************************************
	> File Name: epoll.h
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Thu 31 Jul 2014 05:43:24 AM PDT
 ************************************************************************/

#ifndef EPOLL_H_
#define EPOLL_H_

#include <sys/epoll.h>

#define ERR_EXIT(m)\
	do{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)


typedef struct{
	struct epoll_event events_[2048];
	int epollfd_;
	int listenfd_;
	int nready_;
	void (*handle_callback_)(int,char *);
}epoll_t;


void epoll_init(epoll_t *epoll,int listenfd,void (*handler)(int,char*));
void epoll_handle(epoll_t *epoll);
void epoll_handle_accept(epoll_t *epoll);
void epoll_handle_data(epoll_t *epoll,int peerfd);
void epoll_close(epoll_t *epoll);

#endif
