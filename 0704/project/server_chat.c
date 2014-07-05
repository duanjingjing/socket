/*************************************************************************
	> File Name: server_chat.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Fri 04 Jul 2014 06:36:56 AM PDT
 ************************************************************************/

#include "chat.h"

void msg_on(pUSR* phead,SA* addr);
void msg_off(pUSR* phead,SA* addr);
void msg_usr(int fd_server,MSG* msg);
void msg_grp(int fd_server,pUSR head,MSG* msg);

int main(int argc,char *argv[]){
    if(argc!=3){
		printf("USAGE: EXE IP PORT");
		exit(-1);
	}

	int server_fd;
	server_fd=socket(AF_INET,SOCK_DGRAM,0);
	if(server_fd==-1){
		perror("socket");
		exit(-1);
	}
    
	pUSR pList=NULL;
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));

	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr=inet_addr(argv[1]);

	if(-1==bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))){
		perror("bind");
		exit(-1);
	}

	fd_set read_set,ready_set;
	FD_ZERO(&read_set);
	FD_SET(server_fd,&read_set);
    struct timeval tm;

	while(1){
       tm.tv_sec=0;
	   tm.tv_usec=1000;
	   MSG chat_msg;
	   bzero(&chat_msg,sizeof(MSG));
    
	   ready_set=read_set;
	   select(server_fd+1,&ready_set,NULL,NULL,&tm);
	   if(FD_ISSET(server_fd,&ready_set)){
		   SA from_addr;
		   int addrlen=sizeof(from_addr);
		   bzero(&from_addr,sizeof(MSG));
		   recvfrom(server_fd,&chat_msg,sizeof(MSG),0,(struct sockaddr*)&from_addr,&addrlen);
           printf("recv \n",chat_msg.s_msg);
		   switch(chat_msg.s_type){
			   case 1:
                   msg_usr(server_fd,&chat_msg);
				   break;
			   case 2:
				   msg_grp(server_fd,pList,&chat_msg);
				   break;
			   case 3:
				   msg_on(&pList,&from_addr);
				   break;
			   case 4:
				   msg_off(&pList,&from_addr);
				   break;
		   }
	   }
	}
}

void msg_on(pUSR* phead,SA* addr){
     printf("ip:port ON! %s:%d\n",inet_ntoa(addr->sin_addr),ntohs(addr->sin_port));
	 pUSR pCur=(pUSR)calloc(1,sizeof(USR));
	 pCur->s_addr=*addr;
	 pCur->s_next=*phead;
	 *phead=pCur;
}

void msg_off(pUSR* phead,SA* addr){
	pUSR pPre,pCur;
	pPre=NULL,pCur=*phead;

	while(phead){
       if((pCur->s_addr).sin_port==addr->sin_port && (pCur->s_addr).sin_addr.s_addr==addr->sin_addr.s_addr){
		   break;
	   }

	   else{
		   pPre=pCur;
		   pCur=pCur->s_next;
	   }
	}

	if(pPre==NULL){
		*phead=(*phead)->s_next;
		free(pCur);
	}
	else{
        pPre->s_next=pCur->s_next;
		free(pCur);
	}
   printf("ip:port OFF! %s:%d\n",inet_ntoa(addr->sin_addr),ntohs(addr->sin_port));
	
}

void msg_usr(int server_fd,MSG* msg){
	sendto(server_fd,msg->s_msg,msg->s_len,0,(struct sockaddr*)&msg->s_addr,sizeof(SA));
}


void msg_grp(int server_fd,pUSR head,MSG* msg){
	while(head){
	   sendto(server_fd,msg->s_msg,msg->s_len,0,(struct sockaddr*)&(head->s_addr),sizeof(SA));
	   head=head->s_next;
	}
}
