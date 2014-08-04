/*************************************************************************
	> File Name: pool.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Mon 07 Jul 2014 08:36:47 PM PDT
 ************************************************************************/

#include "pool.h"

void handle_request(int sockfd,SA* addr,char* buf){
     sendto(sockfd,buf,strlen(buf),0,(struct sockaddr*)addr,sizeof(SA));
}

static void child_main();

int make_child(pCHLD arr,int cnt){
	int index;
	int max_read=0x80000000;

	for(index=0;index<cnt;index++){
       pid_t pid;
	   int fds1[2],fds2[2];
	   pipe(fds1);//pr cw
	   pipe(fds2);//cr pw
       
	   pid=fork();
	   if(pid==0){
          close(fds1[0]);
		  close(fds2[1]);
		  dup2(fds1[1],1);
		  dup2(fds2[0],0);

		  child_main();
		  exit(0);
	   }

	   else{
          close(fds1[1]);
		  close(fds2[0]);

		  arr[index].s_pid=pid;
		  arr[index].s_flag=S_IDLE;
		  arr[index].s_read=fds1[0];
		  if(fds1[0]>max_read){
			  max_read=fds1[0];
		  }

		  arr[index].s_write=fds2[1];
	   }
	}
	return max_read;
}

void child_main(){
	int fd;
	while(1){
		SA client_addr;
		int fd=socket(AF_INET,SOCK_DGRAM,0);
        char buf[1024];
		scanf("%d %d",(int*)&(client_addr.sin_port),&(client_addr.sin_addr.s_addr));
		fgets(buf,1024,stdin);
        buf[strlen(buf)-1]='\0';
		handle_request(fd,&client_addr,buf);
		printf("1");
		fflush(stdout);
	}
}
