/*************************************************************************
	> File Name: ftp_nobody.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 27 Aug 2014 10:08:37 PM PDT
 ************************************************************************/

#include "ftp_nobody.h"


void set_nobody(){
	//基本思路
	//1.首先获取nobody的uid、gid
	//2.然后逐项进行设置
	struct passwd *pw;
    if((pw=getpwnam("nobody"))==NULL){
		ERR_EXIT("getpwnam");
	}
	
	if(seteuid(pw->pw_uid)==-1)
		ERR_EXIT("setuid");

	if(setegid(pw->pw_gid)==-1)
		ERR_EXIT("setgid");

}

//nobody时刻准备从子进程接收命令
void handle_nobody(session_t *sess){
     char cmd;
	 while(1){
		 int ret=readn(sess->nobody_fd,&cmd,sizeof(cmd));
		 if(ret==-1){
			 if(errno==EINTR) continue;
			 ERR_EXIT("readn");
		 }

		 else if(ret==0){
			 exit(EXIT_SUCCESS);
		 }

		 else{
			 //
		 }
	 }
}

