/*************************************************************************
	> File Name: TcpConnection.cpp
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 06 Aug 2014 05:55:45 AM PDT
 ************************************************************************/

#include "TcpConnection.h"
#include <stdlib.h>
using namespace std;

#define ERR_EXIT(m)\
	do{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

TcpConnection::TcpConnection(int sockfd):sockfd_(sockfd),
	buffer_(sockfd){
}

TcpConnection::~TcpConnection(){
	shutdown();
}

ssize_t TcpConnection::readn(char *usrbuf,size_t n){
	int nread=buffer_.readn(usrbuf,n);
	if(nread==-1) ERR_EXIT("readn");
	return nread;
}

ssize_t TcpConnection::readline(char*usrbuf,size_t n){
	int nread=buffer_.readline(usrbuf,n);
	if(nread==-1)
		ERR_EXIT("readline");
	return nread;
}

ssize_t TcpConnection::writen(const char* usrbuf,size_t n){
	int nwrite=buffer_.writen(usrbuf,n);
	if(nwrite==-1){
		ERR_EXIT("writen");
	}

	return nwrite;
}

void TcpConnection::send(const std::string &s){
	writen(s.c_str(),s.size());
}

void TcpConnection::shutdown(){
	sockfd_.shutdownWrite();
}


