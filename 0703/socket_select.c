/*************************************************************************
	> File Name: socket_select.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Thu 03 Jul 2014 06:20:55 AM PDT
 ************************************************************************/

#include<stdio.h>
#include "tcp_net_socket.h"

#define MAXCLIENT 10

int main(){
	int sfd=tcp_init("192.168.0.164",8888);
	int fd=0;
	char buf[512]={0};
	fd_set rdset;
	while(1){
		FD_ZERO(&rdset);
		FD_SET(sfd,&rdset);
		if(select(MAXCLIENT+1,&rdset,NULL,NULL,NULL)<0){
			continue;
		}

		for(fd=0;fd<MAXCLIENT;fd++){
		    if(fd==sfd){
				int cfd=tcp_accept(sfd);
				FD_SET(cfd,&rdset);
			}

			else{
				bzero(buf,sizeof(buf));
				recv(fd,buf,sizeof(buf),0);
				puts(buf);
				send(fd,"java",5,0);

				close(fd);
			}
		}

	}
}
