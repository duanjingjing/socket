/*************************************************************************
	> File Name: epoll_client.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 30 Jul 2014 02:08:19 AM PDT
 ************************************************************************/

#include "network.c"
#include <sys/epoll.h>

static void do_epoll(int fd){
	char recvbuf[MAXLINE+1];
	char sendbuf[MAXLINE+1];
	memset(recvbuf,0x00,sizeof(recvbuf));
	memset(sendbuf,0x00,sizeof(sendbuf));

	int epollfd=epoll_create(2);
	if(epollfd==-1){
		ERR_EXIT("epoll_create");
	}

	struct epoll_event events[2];
	struct epoll_event ev;
	int stdin_fd=fileno(stdin);
	ev.data.fd=stdin_fd;
	ev.events=EPOLLIN;
	int ret=epoll_ctl(epollfd,EPOLL_CTL_ADD,stdin_fd,&ev);
	if(ret==-1){
		ERR_EXIT("epoll_ctl");
	}

	ev.data.fd=fd;
	ev.events=EPOLLIN;
	ret=epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
	if(ret==-1){
		ERR_EXIT("epoll_ctl");
	}

	int nready;
	while(1){
		nready=epoll_wait(epollfd,events,2,-1);
		if(nready<0){
			ERR_EXIT("epoll_wait");
		}
		else if(nready==0){
			continue;
		}

		int i;
		for(i=0;i<nready;i++){
			if(events[i].data.fd==stdin_fd){
				if(fgets(sendbuf,MAXLINE,stdin)==NULL){
					ev.data,fd=stdin_fd;
					epoll_ctl(epollfd,EPOLL_CTL_DEL,stdin_fd,&ev);
					shutdown(fd,SHUT_WR);
				}
				else{
					if(strcmp(sendbuf,"\n")==0){
						continue;
					}
					else{
						writen(fd,sendbuf,strlen(sendbuf));
						memset(sendbuf,0x00,sizeof(sendbuf));
					}
				}
			}

			if(events[i].data.fd==fd){
				int ret=readline(fd,recvbuf,MAXLINE);
				if(ret<0){
					ERR_EXIT("readline");
				}
				else if(ret==0){
					ev.data.fd=fd;
					ev.events=EPOLLIN;
					epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);
					close(epollfd);
					exit(EXIT_SUCCESS);
				}

				fprintf(stdout,"receive:%s",recvbuf);
				memset(recvbuf,0x00,sizeof(recvbuf));
			}
		}
	}
}


int main(int argc,char *argv[]){
	int listenfd=socket(AF_INET,SOCK_STREAM,0);
	if(listenfd==-1){
		ERR_EXIT("socket");
	}

	struct sockaddr_in peeraddr;
	peeraddr.sin_family=AF_INET;
	peeraddr.sin_port=htons(8888);
	peeraddr.sin_addr.s_addr=inet_addr("127.0.0.1");

	int ret=connect(listenfd,(struct sockaddr*)&peeraddr,sizeof(peeraddr));
	if(ret==-1){
		ERR_EXIT("connect");
	}

   do_epoll(listenfd);
   return 0;
}
