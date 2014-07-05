/*************************************************************************
	> File Name: server_processpool.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Thu 03 Jul 2014 04:32:51 AM PDT
 ************************************************************************/

#include "my_child.h"

int main(int argc,char *argv[]){//exe ip port chl_cnt
	if(argc!=4){
		printf("USAGE: EXE IP PORT CNT");
		exit(-1);
	}

	int fd_server,index;
	fd_set read_set,ready_set;

	pCHLD my_chids=(pCHLD)calloc(atoi(argv[3]),sizeof(CHLD));
	make_child(my_chids,atoi(argv[3]));

	fd_server=listenfd_init(argv[1],argv[2]);
	FD_ZERO(&read_set);
	FD_SET(fd_server,&read_set);

	for(index=0;index<atoi(argv[3]);index++){
		FD_SET(my_chlds[index].s_read,&s_read);
	}

	struct timeval tm;
	while(1){
		tm.tv_usec=1000;
		tm.tv_sec=0;
		ready_set=read_set;
		select(1024,&ready_set,NULL,NULL,&tm);

		if(FD_ISSET(fd_server,&ready_set)){
			int fd_client=accept(fd_server,NULL,NULL);
            find_child_handle();
		}

		for(index=0;index<atoi(argv[3]);index++){
			if(FD_ISSET(my_chlds[index].s_read,ready_set)){
                read_from_fd();
				my_chlds[index].s_stauts=S_IDLE;
			}
		}

		
	}




    
}

