/*************************************************************************
	> File Name: client.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 02 Jul 2014 10:30:39 PM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<signal.h>
#include"msg.h"

int main(int argc,char *argv[]){
	if(argc!=3){
		printf("usage:exe server_ip server_port\n");
		exit(-1);
	}

	int client_fd,file_fd;
	client_fd=socket(AF_INET,SOCK_STREAM,0);
	if(client_fd==-1){
		perror("socket");
		exit(-1);
	}
   int total=0;
	struct sockaddr_in server_addr;
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr=inet_addr(argv[1]);
    MSG msg,rcv_msg;
	if((connect(client_fd,(struct sockaddr*)&server_addr,sizeof(server_addr)))==-1){
		perror("connect");
		close(client_fd);
		exit(-1);
	}

	memset(&msg,0,sizeof(MSG));
	printf("input:");
	scanf("%s",msg.s_buf);
	msg.s_len=strlen(msg.s_buf);
    file_fd=open("client_copy.txt",O_WRONLY|O_CREAT,0666);
	send(client_fd,&msg,msg.s_len+MSG_LEN,0);

	while(1){
		memset(&rcv_msg,0,sizeof(MSG));
		recv(client_fd,&rcv_msg,MSG_LEN,0);
		total+=rcv_msg.s_len;
	//	system("clear");
		printf("downloading...%.2f kb\n",(double)total/1024);

		if(rcv_msg.s_len>0){
			printf("write %d bytes\n",rcv_msg.s_len);
			recv(client_fd,rcv_msg.s_buf,rcv_msg.s_len,0);
			write(file_fd,rcv_msg.s_buf,rcv_msg.s_len);
		}
		else{
			break;
		}
	}
	close(file_fd);
	close(client_fd);
	return 0;

}
