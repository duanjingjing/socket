/*************************************************************************
	> File Name: TcpConnection.h
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 06 Aug 2014 05:51:05 AM PDT
 ************************************************************************/

#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include "NonCopyable.h"
#include "InetAddress.h"
#include "Socket.h"
#include "Rio.h"
#include <string>
#include <memory>

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

class TcpConnection:private NonCopyable{
public:
	typedef std::function<void (const TcpConnectionPtr&)> TcpConnectionCallback;
	explicit TcpConnection(int sockfd);
	~TcpConnection();

	int fd() const{return sockfd_.fd();}

	ssize_t readn(char *usrbuf,size_t n);
	ssize_t readline(char *usrbuf,size_t maxline);
	ssize_t writen(const char* usrbuf,size_t n);
	void send(const std::string &s);
	void shutdown();
private:
	Socket sockfd_;//fd
	Rio buffer_;
};

#endif
