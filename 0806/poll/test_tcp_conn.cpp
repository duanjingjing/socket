/*************************************************************************
	> File Name: test_tcp_conn.cpp
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 06 Aug 2014 06:01:56 AM PDT
 ************************************************************************/
#include "TcpConnection.h"
#include<string>
#include<vector>
#include<iostream>
using namespace std;

#define ERR_EXIT(m) \
	do{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

int get_listen_fd();

int main(int argc,char *argv[]){
	int listenfd=get_listen_fd();
	if(listenfd==-1)
		ERR_EXIT("listenfd");

	int peerfd=accept(listenfd,NULL,NULL);
	if(peerfd==-1) ERR_EXIT("accept");

	TcpConnectionPtr conn(new TcpConnection(peerfd));

	char buf[1024];
	while(1){
		int ret=conn->readline(buf,1024);
		if(ret==0){
			conn->shutdown();
			cout<<"close"<<endl;
			break;
		}
		cout<<"recv data "<<buf<<endl;
		conn->send(buf);
	}

	return 0;
}


int get_listen_fd(){
	//创建socket
	int listenfd=socket(AF_INET,SOCK_STREAM,0);
	if(listenfd==-1){
		ERR_EXIT("socket");
	}

	//设置端口复用
    int on=1;
	if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0){
        ERR_EXIT("setsockopt");
	}

	struct sockaddr_in servaddr;
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(8888);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);

	if(::bind(listenfd,(struct sockaddr*)&servaddr,sizeof(struct sockaddr))<0)
		ERR_EXIT("listen");

	if(listen(listenfd,SOMAXCONN)<0)
		ERR_EXIT("listen");

	return listenfd;
}
