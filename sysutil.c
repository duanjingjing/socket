/*************************************************************************
	> File Name: sysutil.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 27 Aug 2014 07:08:23 PM PDT
 ************************************************************************/

#include "sysutil.h"

int tcp_server(const char* host,unsigned short port){
	int listenfd;
	if((listenfd=socket(AF_INET,SOCK_STREAM,0))==-1)
			ERR_EXIT("tcp_server");
	
	struct sockadd_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	if(host!=NULL){
		if(inet_aton(host,&servaddr.sin_addr)==0){
			struct hostent *hp;
			hp=gethostbyname(host);
			if(hp==NULL)
				ERR_EXIT("gethostbyname");

			servaddr.sin_addr=*(struct in_addr*)hp->h_addr;
		}
	}

	else
		servaddr.sin_addr.s_addr=htons(INADDR_ANY);

	servaddr.sin_port=htons(port);

	int on=1;
	if((setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(const char*)&on,sizeof(on)))<0){
		ERR_EXIT("bind");
	}

	if(listen(listenfd,SOMAXCONN)<0)
		ERR_EXIT("listen");
	return listenfd;
}

int getlocalip(char* ip){
	char host[100]={0};
	if(gethostname(host,sizeof(host))<0)
		return -1;

	struct hostent *hp;
	if((hp=gethostbyname(host))==NULL)
		return -1;

	strcpy(ip,inet_ntoa(*(struct in_addr*)hp->h_addr));
	return 0;
}

/*
 *activate_noblock -设置I/O为非阻塞模式
 *@fd:文件描述符
 */

void activate_nonblock(int fd){
	 int ret;
	 int flags=fcntl(fd,F_GETFL);
	 if(flags==-1)
		 ERR_EXIT("fcntl");

	 flags|=O_NONBLOCK;
	 ret=fcntl(fd,F_SETFL,flags);
	 if(ret==-1)
		 ERR_EXIT("fcntl");
}

void deactivate_nonblock(int fd){
	int ret;
	int flags=fcntl(fd,F_GETFL);
	if(flags==-1)
		ERR_EXIT("fcntl");

	flags&=~O_NONBLOCK;
	ret=fcntl(fd,F_SETFL,flags);
	if(ret==-1)
		ERR_EXIT("fcntl");
}


/*
 *read_timeout -读超时检测函数，不含读操作
 *@wait_seconds：等待超时秒数,如果为0表示不检测超时
 *
 */

int read_timeout(int fd,unsigned int wait_seconds){
	int ret=0;
	if(wait_seconds>0){
		fd_set read_fdset;
		struct timeval timeout;

		FD_ZERO(&read_fdset);
		FD_SET(fd,&read_fdset);

		timeout.tv_sec=wait_seconds;
		timeout.tv_usec=0;

		do{
			ret=select(fd+1,&read_fdset,NULL,NULL,&timeout);
		}while(ret<0 && errno==EINTR);

		if(ret==0){
			ret=-1;
			errno=ETIMEOUT;
		}

		else if(ret==-1)
			ret=0;
	}
	return ret;
}


/**
 *write_timeout -读超时检测函数,不含写操作
 *@fd：文件描述符
 *@wait_seconds:等待超时秒数,如果为0表示不检测超时
*成功(未超时)返回0，失败返回-1,超时返回-1并且errno=ETIMEOUT
*/

int write_timeout(int fd,unsigned int wait_seconds){
	int ret=0;
	if(wait_seconds>0){
		fd_set write_fdset;
		struct timeval timeout;

		FD_ZERO(&write_fdset);
		FD_SET(fd,&write_fdset);

		timeout.tv_sec=wait_seconds;
		timeout.tv_usec=0;

		do{
			ret=select(fd+1,NULL,NULL,&write_fdset,&timeout);
		}while(ret<0 && errno==EINTR);

		if(ret==0){
			ret=-1;
			errno=ETIMEOUT;
		}

		else if(ret==-1){
			ret=0;
		}
	}
	return ret;
}

/**
 *accept_timeout -带超时的accept
 *
 */

int accept_timeout(int fd,struct sockaddr_in *addr,unsigned int wait_seconds){
	int ret;
	sockelen_t addrlen=sizeof(struct sockaddr_in);

	if(wait_seconds>0){
		fd_set accept_fdset;
		struct timeval timeout;
		FD_ZERO(&accept_fdset);
		FD_SET(fd,&accept_fdset);

		timeout.tv_sec=wait_seconds;
		timeout.tv_usec=0;

		do{
			ret=select(fd+1,&accept_fdset,NULL,NULL,&timeout);
		}while(ret<0 && errno==EINTR);

		if(ret==-1)
			return -1;
		else if(ret==0){
			errno=ETIMEOUT;
			return -1;
		}
	}

	if(addr!=NULL)
		ret=accpet(fd,(struct sockaddr*)addr,&addrlen);
	else
	    ret=accept(fd,NULL,NULL);

	if(ret==-1){
	}
	
	return ret;
}

/**
 *connect_timeout -connect
 *@fd:套接字
 *@addr:要连接的对方地址
 *@wait_seconds:等待超时秒数,如果为0表示正常模式
 *成功(未超时)返回0，失败返回-1，超时返回-1
 */

int connect_timeout(int fd,struct sockaddr_in* addr,unsigned int wait_seconds){
    int ret;
	socklen_t addrlen;

	if(wait_seconds>0)
		activate_nonblock(fd);

	
}




