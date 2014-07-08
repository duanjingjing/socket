/*************************************************************************
	> File Name: server.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 02 Jul 2014 09:58:12 PM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include"msg.h"

int main(int argc,char *argv[]){
	if(argc!=3){
		printf("usage: exe server_ip port!\n");
		exit(-1);
	}

    int reuse=1;
	int server_fd,client_fd,file_fd;
	server_fd=socket(AF_INET,SOCK_STREAM,0);
	if(server_fd==-1){
		perror("socket");
		exit(-1);
	}
    if(0!=setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int))){
		perror("setsockopt");
		exit(-1);
	}

	int read_n;
	struct sockaddr_in server_addr;
	memset(&server_addr,0,sizeof(server_addr));
    MSG msg,snd_msg;
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr=inet_addr(argv[1]);

	if(-1==bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))){
		perror("bind");
		close(server_fd);
		exit(-1);
	}

	if(-1==listen(server_fd,5)){
		perror("listen");
		close(server_fd);
		exit(-1);
	}

    struct sockaddr_in  client_addr;
	int addrlen=sizeof(client_addr);

    while(client_fd=accept(server_fd,(struct sockaddr*)&client_addr,&addrlen)){
        if(client_fd==-1){
			perror("accept");
			exit(-1);
		}
        printf("someone connecting...\n");
		memset(&msg,0,sizeof(MSG));
		recv(client_fd,&msg,MSG_LEN,0);
		recv(client_fd,&msg.s_buf,msg.s_len,0);
        
		printf("recv msg:%s\n",msg.s_buf);

		file_fd=open(msg.s_buf,O_RDONLY);
        printf("open file success!\n");

		while(memset(&snd_msg,0,sizeof(snd_msg)),(read_n=read(file_fd,snd_msg.s_buf,MSG_SIZE))>0){
			  printf("read_n:%d\n",read_n);
              snd_msg.s_len=read_n;
			  send(client_fd,&snd_msg,snd_msg.s_len+MSG_LEN,0);
		}
       
		snd_msg.s_len=0;
		send(client_fd,&snd_msg,snd_msg.s_len+MSG_LEN,0);
		close(file_fd);
		close(client_fd);
			printf("write %d bytes\n",rcv_msg.s_len);
	} 
}
