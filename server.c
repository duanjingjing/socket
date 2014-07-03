/*************************************************************************
  > File Name: server.c
  > Author: canjian
  > Mail:410001089@qq.com 
  > Created Time: Wed 02 Jul 2014 05:49:06 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>

#define IP "192.168.112.128"
#define PORT 8888

int main(){
	int server_fd,iRet,client_fd;
	server_fd=socket(AF_INET,SOCK_STREAM,0);
	if(server_fd==-1){
		perror("socket");
		close(server_fd);
		exit(-1);
	}

	struct sockaddr_in server_addr;
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(PORT);
	server_addr.sin_addr.s_addr=inet_addr(IP);
	iRet=bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));

	if(iRet==-1){
		perror("bind");
		close(server_fd);
		exit(-1);
	}
	printf("bind success!...\n");

	iRet=listen(server_fd,5);
	if(iRet==-1){
		perror("listen");
		close(server_fd);
		exit(-1);
	}

	printf("listen success!...\n");

	while(1){
		struct sockaddr_in client_addr;
		memset(&client_addr,0,sizeof(client_addr));
		int len=sizeof(client_addr);
		client_fd=accept(server_fd,(struct sockaddr*)&client_addr,&len);

		char buf[128]={0};
		int read_n=recv(client_fd,buf,128,0);
		if(read_n==-1){
			printf("recv error!\n");
			break;
		}
		printf("recv from %s\n MSG: %s\n Len:%d\n",inet_ntoa(client_addr.sin_addr),
				buf,read_n);

		char *p="He Xuejun";
		int send_n=send(client_fd,p,strlen(p),0);
		printf("send to IP: %s:%d\n Msg:%s\n Len:%d\n",inet_ntoa(client_addr.sin_addr),
				ntohs(client_addr.sin_port),p,send_n);
	}

	close(server_fd);
	close(client_fd);

	return 0;
}
