/*************************************************************************
	> File Name: session.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 27 Aug 2014 09:54:42 PM PDT
 ************************************************************************/

#include "session.h"

void session_init(session* sess){
	memset(sess->command,0,sizeof(command));
	memset(sess->comm,0,sizeof(comm));
	memset(sess->args,0,sizeof(args));

	sess->peerfd=-1;
	sess->nobody_fd=-1;
	sess->proto_fd=-1;
}

void session_begin(session_t *sess){
	int fds[2];
	if(socketpair(PF_UNIX,SOCK_STREAM,0,fds)==-1){
		ERR_EXIT("socketpair");
	}

	pid_t pid;

	if((pid=fork())==-1){
		ERR_EXIT("fork");
	}

	else if(pid==0){
       close(fds[0]);
	   sess->proto_fd=fds[1];
	   handle_proto(sess);
	}

	else{
	   close(fds[1]);
	   sess->nobody_fd=fds[0];
	   handle_nobody(sess);
	}
}
