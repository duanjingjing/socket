/*************************************************************************
	> File Name: client.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 02 Jul 2014 06:12:43 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<arpa/inet.h>

#define IP "192.168.1.35"
#define PORT 8888

int main(){
    int client_fd;
	client_fd=socket(AF_INET,SOCK_STREAM,0);
	if(client_fd==-1){
		perror("socket");
		close(client_fd);
		exit(-1);
	}
    int iRet;
	struct sockaddr_in server_addr;
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(PORT);
	server_addr.sin_addr.s_addr=inet_addr(IP);

	iRet=connect(client_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(iRet==-1){
		printf("connect failure!\n");
		exit(-1);
	}
	printf("connect success!...\n");

    char *p="i miss you!";
    int send_n= send(client_fd,p,strlen(p),0);
	if(send_n==-1){
		printf("send failure!\n");
		exit(-1);
	}

	char buf[128]={0};
	int recv_n=recv(client_fd,buf,128,0);

	printf("recv from ip:port %s:%d\n Msg:%s\n len:%d\n",inet_ntoa(server_addr.sin_addr),
			ntohs(server_addr.sin_port),buf,recv_n);

	close(client_fd);
	return 0;
}

