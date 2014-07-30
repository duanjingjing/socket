/*************************************************************************
	> File Name: server1.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Tue 29 Jul 2014 02:13:23 AM PDT
 ************************************************************************/
#include "network.c"
#include <sys/select.h>

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

static void do_select(int fd);


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

	do_select(listenfd);
	close(listenfd);
	return 0;
}

static void do_select(int listenfd){
	//准备一些初始化参数
	fd_set allset;//备份rset
	fd_set rset;

	int clients[FD_SETSIZE];//保存已连接的客户fd
    int i;
	for(i=0;i<FD_SETSIZE;i++){
		clients[i]=-1;
	}
    
	int maxi=-1;//代表clients中的最大下标
	int nready;
	int maxfd=listenfd;
	FD_ZERO(&allset);
	FD_ZERO(&rset);
	FD_SET(listenfd,&allset);
    
	while(1){
		//初始化rset
		rset=allset;
		//执行select调用
		//rset既是输入参数，也是输出参数
		nready=select(maxfd+1,&rset,NULL,NULL,NULL);
		if(nready<0){
			if(errno==EINTR) continue;
			ERR_EXIT("select");
		}
		else if(nready==0) continue;

		//判断listenfd
		if(FD_ISSET(listenfd,&rset)){
            //accept
			int peerfd=accept(listenfd,NULL,NULL);
			if(peerfd==-1){
				ERR_EXIT("accept");
			}

			//加入client同时处理client满的请求
			int i;
			for(i=0;i<FD_SETSIZE;i++){
				if(clients[i]==-1){
					clients[i]=peerfd;
					if(maxi<i) maxi=i;//最大下标需要更新
					break;
				}
			}

			if(i==FD_SETSIZE){
				fprintf(stdout,"too many clients\n");
				exit(EXIT_FAILURE);
			}
			//加入allset中
			FD_SET(peerfd,&allset);
			if(peerfd>maxfd){
				maxfd=peerfd;
			}
			if(--nready==0){
				continue;
			}
		}


		//轮询客户fd
        int i;
		for(i=0;i<=maxi;i++){
			if(FD_ISSET(clients[i],&rset)){
				char recvbuf[1024];
				int fd=clients[i];
				int ret=readline(fd,recvbuf,1024);
				if(ret<0){
					ERR_EXIT("readline");
				}
				else if(ret==0){
					fprintf(stdout,"peer close\n");
					close(fd);
					clients[i]=-1;
					FD_CLR(fd,&allset);
				}

				printf("recv data:%s",recvbuf);
				writen(fd,recvbuf,strlen(recvbuf));
			}

		}

	}
}
