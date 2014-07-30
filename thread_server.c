/*************************************************************************
	> File Name: server.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Mon 28 Jul 2014 12:21:14 AM PDT
 ************************************************************************/
#include "network.c"
#include<signal.h>
#include<pthread.h>

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


static void* thread_func(void *arg);

int main(int argc ,char *argv[]){
	if(signal(SIGPIPE,SIG_IGN)==SIG_ERR){
		ERR_EXIT("signal");
	}

	int listenfd=socket(AF_INET,SOCK_STREAM,0);
	if(listenfd==-1){
		perror("socket");
		exit(-1);
	}

	int on=1;
	if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0){
		perror("setsockopt");
		exit(-1);
	}

	struct sockaddr_in serveraddr;
    serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(9999);
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");

	if(-1==bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr_in))){
		perror("bind");
		exit(-1);
	}

	if(-1==listen(listenfd,10)){
		perror("listen");
		exit(-1);
	}
    
    while(1){
        
        int peerfd=accept(listenfd,NULL,NULL);
		if(peerfd==-1){
			ERR_EXIT("accept");
		}
        //每接收一个请求，就创建一个新的线程
		pthread_t tid;
		pthread_create(&tid,NULL,thread_func,(void*)peerfd);
    }
	close(listenfd);
	return 0;
}

void* thread_func(void *arg){

	pthread_detach(pthread_self());//把自己设置为分离状态
	int peerfd=(int)arg;
	do_service(peerfd);
	close(peerfd);
}
