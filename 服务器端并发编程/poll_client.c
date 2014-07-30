/*************************************************************************
	> File Name: pool_client.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 30 Jul 2014 12:11:43 AM PDT
 ************************************************************************/

#include "network.c"
#include <poll.h>


static void do_service(int fd){
	char recvbuf[MAXLINE+1];
	char sendbuf[MAXLINE+1];
	memset(recvbuf,0,sizeof(recvbuf));
	memset(sendbuf,0,sizeof(sendbuf));

	struct pollfd clients[2];
	clients[0].fd=fileno(stdin);
	clients[0].events=POLLIN;
	clients[1].fd=fd;
	clients[1].events=POLLIN;

	int nready;
	int maxi=1;
	while(1){
		nready=poll(clients,maxi+1,-1);
		if(nready<0){
			if(errno==EINTR) continue;
			ERR_EXIT("poll");
		}
		else if(nready==0){
			continue;
		}

		if(clients[0].revents & POLLIN){
			if(fgets(sendbuf,1024,stdin)==NULL){
                clients[0].fd=-1;
				shutdown(fd,SHUT_WR);
			}

			else{
				writen(fd,sendbuf,strlen(sendbuf));
				memset(sendbuf,0x00,sizeof(sendbuf));
			}
		}

		if(clients[1].revents & POLLIN){
			int ret=readline(fd,recvbuf,MAXLINE);
			if(ret<0){
				ERR_EXIT("readline");
			}
			else if(ret==0){
				fputs("peer close\n",stdout);
				clients[1].fd=-1;
				break;
			}

			fprintf(stdout,"receive:%s",recvbuf);
			memset(recvbuf,0,sizeof(recvbuf));
		}
	}
}


int main(int argc,char *argv[]){
	int cfd=socket(AF_INET,SOCK_STREAM,0);
	if(cfd==-1){
		ERR_EXIT("socket");
	}

	struct sockaddr_in peeraddr;
	peeraddr.sin_family=AF_INET;
	peeraddr.sin_port=htons(8888);
	peeraddr.sin_addr.s_addr=inet_addr("127.0.0.1");

	int ret=connect(cfd,(struct sockaddr*)&peeraddr,sizeof(peeraddr));
	if(ret==-1){
		ERR_EXIT("connect");
	}

    do_service(cfd);
	close(cfd);
	return 0;
}
