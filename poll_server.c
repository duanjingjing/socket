/*************************************************************************
	> File Name: server1.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Tue 29 Jul 2014 02:13:23 AM PDT
 ************************************************************************/
#include "network.c"
#include <sys/select.h>
#include <poll.h>

#define MAXLINE 1023

#define ERR_EXIT(m) \
	do{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)


void do_poll(int listenfd){
	struct pollfd clients[2048];
	int i;
	for(i=0;i<2048;i++){
		clients[i].fd=-1;
	}

	clients[0].fd=listenfd;
	clients[0].events=POLLIN;

    int maxi=0;//最大的数组下标
	int nready;

	while(1){
		//poll
       nready=poll(clients,maxi+1,-1);
	   if(nready==-1){
		   if(errno==EINTR) continue;
		   ERR_EXIT("poll");
	   }
	   else if(nready==0){
		   continue;
	   }
		//listenfd

	   if(clients[0].revents & POLLIN){
           int peerfd=accept(clients[0].fd,NULL,NULL);
		   if(peerfd==-1){
			   ERR_EXIT("accept");
		   }

		   int i=1;
		   for(i=1;i<2048;i++){
			   if(clients[i].fd==-1){
				   clients[i].fd=peerfd;
				   clients[i].events=POLLIN;
				   if(i>maxi) maxi=i;
				   break;
			   }
		   }

		   if(i==2048){
			   fprintf(stdout,"too many clients\n");
			   exit(EXIT_FAILURE);
		   }

		   if(--nready<=0) continue;

	   }
		//clients
		int i;
		for(i=1;i<=maxi;i++){
			int fd=clients[i].fd;
			if(fd==-1){
				continue;
			}
            
			char recvbuf[1024];
			memset(recvbuf,0x00,sizeof(recvbuf));

            if(clients[i].revents & POLLIN){
                int ret=readline(fd,recvbuf,1024);
				if(ret<0){
					ERR_EXIT("readline");
				}
				else if(ret==0){
					close(fd);
					clients[i].fd=-1;
					fprintf(stdout,"peer close\n");
					continue;
				}

				printf("recv:%s",recvbuf);
				writen(fd,recvbuf,strlen(recvbuf));
				
				if(--nready<=0){
					break;
				}
			}
		}
	}
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

	do_poll(listenfd);

	close(listenfd);
	return 0;
}


