/*************************************************************************
	> File Name: network.h
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Thu 31 Jul 2014 01:54:55 AM PDT
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
#define ERR_EXIT(m) \
	do{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

ssize_t readn(int fd,void *usrbuf,size_t n);
ssize_t writen(int fd,void *usrbuf,size_t n);
ssize_t recv_peek(int sockfd,void *usrbuf,size_t n);
ssize_t readline(int sockfd,void *usrbuf,size_t maxline);

