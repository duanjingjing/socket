/*************************************************************************
	> File Name: chat_client2.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Fri 04 Jul 2014 01:17:17 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/in.h>
#include<pthread.h>

int main(int argc,char *argv[]){
	if(argc!=3){
		printf("USAGE:EXE SERVER_IP PORT!\n");
		exit(-1);
	}

	int server_fd;
	struct sockaddr_in server_addr,client_addr;
	server_fd=socket(AF_INET,SOCK_DGRAM,0);
	if(server_fd==-1){
		perror("socket");
		exit(-1);
	}

	char chat_ip[32];
	int ips[4];
	int chat_port;
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr=inet_addr(argv[1]);
    chat_port=atoi(argv[2]);
	if(-1==bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))){
		perror("bind");
		exit(-1);
	}

	while(printf("who(ip):"),fflush(stdout),bzero(chat_ip,32),fgets(chat_ip,32,stdin)!=NULL){
		if(chat_ip[0]=='\n'){
			continue;
		}
		else{
			if(sscanf(chat_ip,"%d.%d.%d.%d",&ips[0],&ips[1],&ips[2],&ips[3])!=4){
				printf("wrong format\n");
				continue;
			}

			if(ips[0]>255 || ips[1]>255 || ips[2]>255 || ips[3]>255){
				printf("wrong format\n");
				continue;
			}

			bzero(&client_addr,sizeof(client_addr));
			client_addr.sin_family=AF_INET;
			client_addr.sin_port=htons(chat_port);
			chat_ip[strlen(chat_ip)-1]='\0';
			client_addr.sin_addr.s_addr=inet_addr(chat_ip);

			pthread_t tid;
			pthread_create(&tid,NULL,recv_func,(void*)server_fd);

			while(memset(chat_msg,0,sizeof(chat_msg)),fgets(chat_msg,1024,stdin)!=NULL)){
				sendto()
			}
		}
	}
}
