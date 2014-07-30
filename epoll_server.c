/*************************************************************************
	> File Name: server1.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Tue 29 Jul 2014 02:13:23 AM PDT
 ************************************************************************/
#include "network.c"
#include <sys/select.h>
#include <poll.h>
#include<sys/epoll.h>

#define MAXLINE 1023

#define ERR_EXIT(m) \
	do{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

void do_epoll(int listenfd){
	//创建epoll
	int epollfd=epoll_create(2048);
	if(epollfd==-1){
		ERR_EXIT("epoll_create");
	}

	//添加listenfd
	struct epoll_event ev;
	ev.data.fd=listenfd;
	ev.events=EPOLLIN;
	if(epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&ev)==-1){
		ERR_EXIT("epoll_ctl");
	}

	//创建数组
	struct epoll_event events[2048];
	int nready;

	while(1){
		nready=epoll_wait(epollfd,events,2048,-1);
		if(nready==-1){
			ERR_EXIT("epoll");
		}

	    int i;
		for(i=0;i<nready;i++){
			if(events[i].data.fd==listenfd){
				int peerfd=accept(listenfd,NULL,NULL);
				if(peerfd==-1){
					ERR_EXIT("accept");
				}

				//加入epoll
			    struct epoll_event ev;
				ev.data.fd=peerfd;
				ev.events=EPOLLIN;
				if(epoll_ctl(epollfd,EPOLL_CTL_ADD,peerfd,&ev)==-1){
					ERR_EXIT("epoll_ctl");
				}
			}
			else{
				int peerfd=events[i].data.fd;
				char recvbuf[1024]={0};
				int ret=readline(peerfd,recvbuf,1024);

				if(ret<0){
                   ERR_EXIT("readline");
				}
				else if(ret==0){
					printf("peer close\n");
					struct epoll_event ev;
					ev.data.fd=peerfd;
					ev.events=EPOLLIN;
					if(epoll_ctl(epollfd,EPOLL_CTL_DEL,peerfd,&ev)==-1){
						ERR_EXIT("epoll_ctl");
					}
					close(peerfd);
					continue;
				}
              printf("receive data:%s",recvbuf);
			  writen(peerfd,recvbuf,strlen(recvbuf));
			  memset(recvbuf,0x00,strlen(recvbuf));

			}
		}
	}
	close(epollfd);
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

	do_epoll(listenfd);
	return 0;
}


