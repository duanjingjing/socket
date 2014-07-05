/*************************************************************************
	> File Name: chat.h
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Fri 04 Jul 2014 02:01:06 AM PDT
 ************************************************************************/
#ifndef _CHAT_H_
#define _CHAT_H_
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/time.h>
#include<netinet/in.h>
#include<sys/select.h>
#include<arpa/inet.h>

#define MSG_USR 1
#define MSG_GRP 2
#define MSG_ON 3
#define MSG_OFF 4
#define MSG_SIZE 1024
#define MSG_LEN (8192-MSG_SIZE)

typedef struct sockaddr_in SA;
typedef struct tagmsg{
	int s_type;
	int s_len;
	SA s_addr;
	char s_msg[MSG_SIZE];
}MSG;

typedef struct tagusr{
	SA s_addr;
	struct tagusr *s_next;
}USR,*pUSR;

#endif
