/*************************************************************************
	> File Name: server1.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Tue 29 Jul 2014 02:13:23 AM PDT
 ************************************************************************/
#include "network.c"

#define MAXLINE 1023

#define ERR_EXIT(m) \
	do{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

static void do_service(int peerfd){
      char recvbuf[1024]={0};
      int ret;

	  while(1){
		  ret=readline(peerfd,recvbuf,1024);
		  if(ret==0){
			  close(peerfd);
			  exit(EXIT_SUCCESS);
		  }

		  printf("recv data:%s",recvbuf);
		  writen(peerfd,recvbuf,strlen(recvbuf));
	  }
}

void handler(int sig){
	while(waitpid(-1,NULL,WNOHANG)>0)
		;
	return;
}

int main(int argc,char *argv[]){
    int listenfd=socket(AF_INET,SOCK_STREAM,0);
	if(listenfd<0){
		ERR_EXIT("socket");
	}

	int on=1;
	if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))==-1){
		ERR_EXIT("setsockopt");
	}

	struct sockaddr_in servaddr;
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(8888);
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	socklen_t len=sizeof(servaddr);

	int ret=bind(listenfd,(struct sockaddr*)&servaddr,len);
	if(ret<0){
		ERR_EXIT("bind");
	}

    ret=listen(listenfd,10);
	if(ret<0){
		ERR_EXIT("listen");
	}

	struct sockaddr_in peeraddr;
	memset(&peeraddr,0,sizeof(peeraddr));
	len=sizeof(peeraddr);

    while(1){
		struct sockaddr_in peeraddr;
		memset(&peeraddr,0,sizeof(peeraddr));
		socklen_t len=sizeof(peeraddr);

		int peerfd=accept(listenfd,(struct sockaddr*)&peeraddr,&len);
		if(peerfd==-1){
			ERR_EXIT("accept");
		}

		pid_t pid;
		if((pid=fork())<0){
			ERR_EXIT("fork");
		}
		else if(pid==0){
			close(listenfd);
            do_service(peerfd);
			exit(EXIT_SUCCESS);
		}
		close(peerfd);
	}
	close(listenfd);
	return 0;
}
