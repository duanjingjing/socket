/*************************************************************************
	> File Name: client.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 02 Jul 2014 07:41:03 PM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include "msg.h"

int main(int argc,char *argv[]){
	if(argc!=3){
		printf("usage:exe server_ip server_port!\n");
		exit(-1);
	}

	int fd_client,fd_file;
	struct sockaddr_in server_addr;
	if((fd_client=socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("socket");
		exit(-1);
	}

	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr=inet_addr(argv[1]);
	if(-1==connect(fd_client,(struct sockaddr*)&server_addr,sizeof(server_addr))){
		perror("connect");
		close(fd_client);
		exit(-1);
	}
    MSG msg,rcv_msg;
	memset(&msg,0,sizeof(msg));
	printf("input:");
	scanf("%s",msg.s_buf);
	msg.s_len=strlen(msg.s_buf);
	fd_file=open(msg.s_buf,O_WRONLY|O_CREAT,0666);
	send(fd_client,&msg,msg.s_len+MSG_LEN,0);

	while(1){
       memset(&rcv_msg,0,sizeof(MSG));
	   recv(fd_client,&rcv_msg,MSG_LEN,0);
       total+=rcv_msg.s_len;
	   total+=rcv_msg.s_len;
	   system("clear");


	   if(rcv_msg.s_len>0){
		   recv(fd_client,&rcv_msg.s_buf,rcv_msg.s_len,0);
		   write(fd_file,rcv_msg.s_buf,rcv_msg.s_len);
	   }
	   else{
		   break;
	   }
	}
	close(fd_file);
	close(fd_client);
	return 0;
}
