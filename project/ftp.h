/*************************************************************************
	> File Name: ftp.h
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Sun 06 Jul 2014 01:36:03 AM PDT
 ************************************************************************/

#ifndef _FTP_H_
#define _FTP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <dirent.h>
#include <signal.h>
#include <fcntl.h>

#define MSG_LEN 4
#define MSG_SIZE (8192-MSG_LEN)


typedef struct tag{
	int s_len;
	char s_msg[MSG_SIZE];
}MSG;

int tcp_init(const char* ip,int port);
int tcp_accept(int sfd);
int tcp_connect(const char* ip,int port);
void func_ls(int client_fd);
void func_cd(const char* pathname);
void download(int sfd,const char* filename);
void trim_space(char *s);
void upload(int sfd,const char* filename);

#endif
