/*************************************************************************
	> File Name: server.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Thu 31 Jul 2014 05:09:15 AM PDT
 ************************************************************************/

#include "network.h"
#include "poll.h"

int get_listen_fd();

void handler(int fd,char *buf){
     printf("recv data:%s",buf);
	 writen(fd,buf,strlen(buf));
}

int main(int argc,char *argv[]){
	if(signal(SIGPIPE,SIG_IGN)==SIG_ERR){
		ERR_EXIT("signal");
	}

	int listenfd=get_listen_fd();

	poll_t pol;
	poll_init(&pol,listenfd,handler);
	while(1){
		poll_do_wait(&pol);
		poll_handle_accept(&pol);
		poll_handle_data(&pol);
	}

	close(listenfd);
	return 0;
}

int get_listen_fd(){
	int listenfd=socket(AF_INET,SOCK_STREAM,0);
	if(listenfd==-1){
		ERR_EXIT("socket");
	}

	int on=1;
	if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0){
		ERR_EXIT("setsockopt");
	}


	struct sockaddr_in servaddr;
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(8888);
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");

	int ret=bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	if(ret==-1){
		ERR_EXIT("bind");
	}

	ret=listen(listenfd,SOMAXCONN);
	if(ret<0){
		ERR_EXIT("listen");
	}

	return listenfd;

}
