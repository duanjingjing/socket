/*************************************************************************
	> File Name: client1.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Tue 29 Jul 2014 02:29:03 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define ERR_EXIT(m) \
	do{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

#define MAXLINE 1023

ssize_t readn(int fd,void *buf,size_t n){
	size_t nleft=n;
	ssize_t nread;
	char *ptr=buf;

	while(nleft>0){
		nread=read(fd,ptr,nleft);
		if(nread<0){
			if(errno==EINTR){
				continue;
			}
			else ERR_EXIT("read");
		}else if(nread==0){
			break;
		}

		nleft-=nread;
		ptr+=nread;
	}
	return n-nleft;
}

ssize_t writen(int fd,void* buf,size_t n){
	size_t nleft=n;
	ssize_t nwrite;
	char *ptr=buf;

	while(nleft>0){
		nwrite=write(fd,ptr,nleft);
		if(nwrite<0){
			if(errno==EINTR) continue;
			ERR_EXIT("write");
		}else if(nwrite==0){
			continue;
		}

		nleft-=nwrite;
		ptr+=nwrite;
	}
	return n;
}

ssize_t recv_peek(int sockfd,void *buf,size_t len){
	int nread;
	while(1){
		nread=recv(sockfd,buf,len,MSG_PEEK);
		if(nread<0 && errno==EINTR) continue;
		if(nread<0) return -1;
		break;
	}
	return nread;
}

ssize_t readline(int sockfd,void *buf,size_t maxline){
	ssize_t  nread;
	size_t nleft;
	char *ptr;
	int ret;
	int total=0;

	nleft=maxline;
	ptr=buf;

	while(nleft>0){
		ret=recv_peek(sockfd,ptr,nleft);
        if(ret<=0){
           return ret;
		}

		nread=ret;
		int i;
		for(i=0;i<nread;i++){
			if(ptr[i]=='\n'){
				ret=readn(sockfd,ptr,i+1);
				if(ret!=i+1){
					return -1;
				}

				total+=ret;
				return total;
			}
		}

		ret=readn(sockfd,ptr,nread);
		if(ret!=nread){
           return -1;
		}

		nleft-=nread;
		total+=nread;
		ptr+=nread;
	}
	return maxline;
}



