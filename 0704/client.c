/*************************************************************************
	> File Name: client.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Thu 03 Jul 2014 07:46:29 PM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<strings.h>
#include<unistd.h>

int main(int argc,char *argv[]){
    int sfd;
	if(argc!=3){
		printf("USAGE:EXE IP PORT\n");
		exit(-1);
	}

	sfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sfd==-1){
		perror("socket");
		exit(-1);
	}

	struct sockaddr_in toaddr,fromaddr;
	bzero(&toaddr,sizeof(toaddr));

	toaddr.sin_family=AF_INET;
	toaddr.sin_port=htons(atoi(argv[2]));
	toaddr.sin_addr.s_addr=inet_addr(argv[1]);

    char buf[1024];
    bzero(buf,1024);

	sendto(sfd,"hello",6,0,(struct sockaddr*)&toaddr,sizeof(toaddr));
	bzero(&fromaddr,sizeof(fromaddr));
	int len=sizeof(fromaddr);
	recvfrom(sfd,buf,1024,0,(struct sockaddr*)&fromaddr,&len);

	printf("recvive from %s  %d,the message is:%s\n",inet_ntoa(fromaddr.sin_addr),
			ntohs(fromaddr.sin_port),buf);

	close(sfd);
}
