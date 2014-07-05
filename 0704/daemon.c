/*************************************************************************
	> File Name: daemon.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Fri 04 Jul 2014 06:19:14 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>

void Daemon(){
	const int MAXFD=64;
	int i=0;
	if(fork()>0){
		exit(0);
	}
	setsid();//成为新进程组组长和新会话领导，脱离控制终端
	chdir("/");
	umask(0);
	for(;i<MAXFD;i++)
		close(i);
}

int main(){
	Daemon();//成为守护进程
	while(1){
		sleep(1);
	}

	return 0;
}
