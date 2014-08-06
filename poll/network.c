/*************************************************************************
	> File Name: network.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Thu 31 Jul 2014 04:32:33 AM PDT
 ************************************************************************/

#include "network.h"


ssize_t readn(int fd,void *usrbuf,size_t n){
	size_t nleft=n;
	ssize_t nread;
	char *ptr=usrbuf;

	while(nleft>0){
		nread=read(fd,ptr,nleft);
		if(nread<0){
			if(errno==EINTR) continue;
			ERR_EXIT("read");
		}
		else if(nread==0){
		    break;
		}

		nleft-=nread;
		ptr+=nread;
	}
	return n-nleft;
}

ssize_t writen(int fd,void *usrbuf,size_t n){
	size_t nleft=n;
	ssize_t nwriten;
	char *ptr=usrbuf;

	while(nleft>0){
		nwriten=write(fd,ptr,nleft);
		if(nwriten<0){
			if(errno==EINTR) continue;
			ERR_EXIT("write");
		}
		else if(nwriten==0){
			ERR_EXIT("writen");
		}

		nleft-=nwriten;
		ptr+=nwriten;
	}
	return n;
}

//recv_peek选项完成一次正确的读取过程
ssize_t recv_peek(int sockfd,void *buf,size_t len){
	int nread;
	while(1){
		//这个过程只成功调用一次
		nread=recv(sockfd,buf,len,MSG_PEEK);
		if(nread<0 && errno==EINTR){
			continue;
		}
		if(nread<0) return -1;
		break;
	}
	return nread;
}

ssize_t readline(int sockfd,void *buf,size_t maxline){
	 int  nread;
	 int  nleft=maxline-1;
	 char *ptr=buf;
     int total=0;

	 while(nleft>0){
       int ret=recv_peek(sockfd,ptr,nleft);
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
			   ptr+=ret;
			   *ptr=0;
			   return total;
		   }
	   }
      
	   //如果没有发现\n,这些数据应全部接收
	   ret=readn(sockfd,ptr,nread);
	   if(ret!=nread){
		   return -1;
	   }
	   nleft-=nread;
	   total+=nread;
	   ptr+=nread;
	 }
	 *ptr=0;
	 return maxline-1;
}





