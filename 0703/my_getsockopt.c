/*************************************************************************
	> File Name: my_getsockopt.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 02 Jul 2014 06:37:46 PM PDT
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(){
	int sock_fd;
	int recv_size=0,snd_size=0;
	int my_snd=8192;
	int my_recv=8192;
	int len_recv=sizeof(int);
	int len_snd=sizeof(int);
	sock_fd=socket(AF_INET,SOCK_STREAM,0);
	getsockopt(sock_fd,SOL_SOCKET,SO_RECVBUF,&recv_size,&len_recv);
	getsockopt(sock_fd,SOL_SOCKET,SO_SNDBUF,&snd_size,&len_snd);

	printf("before socket recvbuf:%d\n socket sndbuf:%d\n")
}
