/*************************************************************************
	> File Name: thread_server.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Thu 03 Jul 2014 05:52:25 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<erron.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<pthread.h>

#define DEFAULT_SVR_PORT 2828
#define FILE_MAX_LEN 64
char filename[FILE_MAX_LEN+1];

static void* handle_client(void *arg){
	int sock=(int arg);
	char buf[1024];
	int  len;
	printf("begin send\n");
	FILE *file=fopen(filename,"r");
	if(file==NULL){
		close(sock);
		exit;
	}

	if(send(sock,filename,FILE_MAX_LEN,0)==-1){
		perror("send file name\n");
		goto EXIT_THREAD;
	}

	printf("begin send file %s...\n",filename);

	while(!feof(file)){
		len=fread(buf,1,sizeof(buf),file);
		printf("server read %s,len %d\n",filename,len);
		if(send(sock,buf,len,0)<0){
			perror("send file");
			goto EXIT_THREAD;
		}
	}

	EXIT_THREAD:
       if(file)
		   fclose(file);
	   close(sock);
}

int main(int argc,char *argv[]){
	int sockfd,new_fd;

	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	unsigned short port;
	
	if(argc<2){
		printf("need a filename without path\n");
		exit(-1);
	}

	strncpy(filename,argv[1],FILE_MAX_LEN);
	port=DEFAULT_SVR_PORT;

    if(argc>=3){
		port=(unsigned short)atoi(argv[2]);
	}

	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("socket");
		exit(-1);
	}

	memset(&my_addr,0,sizeof(my_addr));
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(port);
	my_addr.sin_addr.s_addr=INADDR_ANY;

	if(bind(sockfd,(struct sockaddr*)&my_addr,sizeof(my_addr))==-1){
        perror("bind");
		exit(-1);
	}

    if(listen(sockfd,10)==-1){
		perror("listen");
		goto EXIT_MAIN;
	}


}

