/*************************************************************************
	> File Name: select_server1.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Tue 29 Jul 2014 11:50:58 PM PDT
 ************************************************************************/

#include "network.c"
#include<sys/select.h>

static void do_select(int listenfd);

int main(int argc,char *argv[]){
	int listenfd;
	listenfd=socket(AF_INET,SOCK_STREAM,0);
	if(listenfd==-1){
       ERR_EXIT("socket");
	}

	struct sockaddr_in peeraddr;
	peeraddr.sin_family=AF_INET;
	peeraddr.sin_port=htons(8888);
	peeraddr.sin_addr.s_addr=inet_addr("127.0.0.1");

	int ret=bind(listenfd,(struct sockaddr*)&peeraddr,sizeof(peeraddr));
	if(ret==-1){
		ERR_EXIT("bind");
	}

	ret=listen(listenfd,10);
	if(ret==-1){
		ERR_EXIT("listen");
	}

	do_select(listenfd);
	return 0;
}

static void do_select(int listenfd){
	 int clients[FD_SETSIZE];
	 int i;
	 for(i=0;i<FD_SETSIZE;i++){
		 clients[i]=-1;
	 }

	 fd_set allset;
	 fd_set rset;
	 FD_ZERO(&allset);
	 FD_ZERO(&rset);
     
	 int nready;
	 int maxi=-1;
	 int maxfd=listenfd;
	 FD_SET(listenfd,&allset);

	 while(1){
		 rset=allset;
         
		 nready=select(maxfd+1,&rset,NULL,NULL,NULL);
         if(nready<0){
			 if(errno==EINTR) continue;
			 ERR_EXIT("select");
		 }else if(nready==0){
			 continue;
		 }

		 if(FD_ISSET(listenfd,&rset)){
			 int peerfd=accept(listenfd,NULL,NULL);
			 if(peerfd==-1){
				 ERR_EXIT("accept");
			 }

			 for(i=0;i<FD_SETSIZE;i++){
				 if(clients[i]==-1){
					 clients[i]=peerfd;
					 if(i>maxi) maxi=i;
					 break;
				 }
			 }

			 if(i==FD_SETSIZE){
				 fprintf(stdout,"too many clients\n");
				 exit(EXIT_FAILURE);
			 }

			 FD_SET(peerfd,&allset);
			 if(maxfd<peerfd) maxfd=peerfd;

			 if(--nready<=0) continue;

		 }


		 for(i=0;i<=maxi;i++){
			 if(FD_ISSET(clients[i],&rset)){
				 char recvbuf[1024];
				 memset(recvbuf,0,1024);
				 int fd=clients[i];
				 int nread=readline(fd,recvbuf,1024);
				 if(nread<0){
					 ERR_EXIT("readline");
				 }
				 else if(nread==0){
					 fprintf(stdout,"peer close\n");
					 close(fd);
					 clients[i]=-1;
					 FD_CLR(fd,&allset);
				 }
				
				 fprintf(stdout,"recv:%s",recvbuf);
				 writen(fd,recvbuf,strlen(recvbuf));
			 }
		 }
	 }
     
}
