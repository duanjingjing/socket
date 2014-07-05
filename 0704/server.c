/*************************************************************************
	> File Name: server.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Thu 03 Jul 2014 08:00:25 PM PDT
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>

int main(int argc,char *argv[]){
	if(argc!=3){
		printf("lack of arguement!");
		exit(-1);
	}

	int server_fd;
	server_fd=socket(AF_INET,SOCK_DGRAM,0);
	if(server_fd==-1){
		perror("socket");
		exit(-1);
	}

	struct sockaddr_in fromaddr;
	bzero(&fromaddr,sizeof(fromaddr));
    int addrlen;
	
	fromaddr.sin_family=AF_INET;
	fromaddr.sin_port=htons(atoi(argv[2]));
	fromaddr.sin_addr.s_addr=inet_addr(argv[1]);

	if(-1==bind(server_fd,(struct sockaddr*)&fromaddr,sizeof(fromaddr))){
		perror("bind");
		exit(-1);
	}
	addrlen=sizeof(struct sockaddr_in);
    char buf[1024];
	while(1){
		struct sockaddr_in client_addr;
	    int len=sizeof(client_addr);
		bzero(&client_addr,sizeof(client_addr));
        bzero(buf,1024);
		recvfrom(server_fd,buf,1024,0,(struct sockaddr*)&client_addr,&addrlen);

		printf("recv from %s  %d,the message is:%s\n",inet_ntoa(client_addr.sin_addr),
				ntohs(client_addr.sin_port),buf);
		sendto(server_fd,"world",6,0,(struct sockaddr*)&client_addr,sizeof(client_addr));

	}
	return 0;

}
