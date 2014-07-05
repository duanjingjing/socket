/*************************************************************************
	> File Name: client_chat.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Fri 04 Jul 2014 07:24:15 AM PDT
 ************************************************************************/

#include "chat.h"
#include<pthread.h>

void* recv_func(void* arg){
	int fd_server=(int)arg;
	char buf[1024];
	while(1){
		bzero(buf,1024);
		recvfrom(fd_server,buf,1024,0,NULL,NULL);
		printf("recv: %s\n",buf);
	}
}

int main(int argc,char *argv[]){
	if(argc!=3){
		printf("USAGE:EXE IP PORT\n");
		exit(-1);
	}

	int client_fd;
	client_fd=socket(AF_INET,SOCK_DGRAM,0);
	if(client_fd==-1){
		perror("socket");
		exit(-1);
	}

	SA server_addr;
	bzero(&server_addr,sizeof(SA));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr=inet_addr(argv[1]);

	MSG msg_on,msg_off,msg_chat;
	msg_on.s_type=MSG_ON;
	sendto(client_fd,&msg_on,sizeof(MSG),0,(struct sockaddr*)&server_addr,
			sizeof(server_addr));

	pthread_t tid;
	pthread_create(&tid,NULL,recv_func,(void*)client_fd);

	while(bzero(&msg_chat,sizeof(MSG)),fgets(msg_chat.s_msg,MSG_SIZE,stdin)!=NULL){
		msg_chat.s_type=MSG_GRP;
		msg_chat.s_len=strlen(msg_chat.s_msg);
		sendto(client_fd,&msg_chat,sizeof(MSG),0,(struct sockaddr*)&server_addr,sizeof(SA));
	}

	bzero(&msg_off,sizeof(MSG));
	msg_off.s_type=MSG_OFF;
	sendto(client_fd,&msg_off,sizeof(msg_off),0,(struct sockaddr*)&server_addr,sizeof(SA));
	close(client_fd);
	return 0;
}

