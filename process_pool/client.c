/*************************************************************************
	> File Name: client.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Mon 07 Jul 2014 09:15:05 PM PDT
 ************************************************************************/

#include"pool.h"

int main(int argc,char *argv[]){
	if(argc!=3){
		printf("EXE:PORT IP\n");
		exit(-1);
	}

   int client_fd=socket(AF_INET,SOCK_DGRAM,0);
   if(client_fd==-1){
	   perror("socket");
	   exit(-1);
   }

   struct sockaddr_in server_addr;
   bzero(&server_addr,sizeof(SA));

   server_addr.sin_family=AF_INET;
   server_addr.sin_port=htons(atoi(argv[2]));
   server_addr.sin_addr.s_addr=inet_addr(argv[1]);

    char buf[1024];
    fgets(buf,1024,stdin);
	sendto(client_fd,buf,strlen(buf),0,(struct sockaddr*)&server_addr,sizeof(SA));
	memset(buf,0,sizeof(buf));
	recvfrom(client_fd,buf,1024,0,NULL,NULL);
	puts(buf);

}

