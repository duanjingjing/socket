/*************************************************************************
	> File Name: ftp.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Sun 06 Jul 2014 07:48:52 AM PDT
 ************************************************************************/

#include "ftp.h"

int tcp_init(const char* ip,int port){
    int sfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd==-1){
		perror("socket");
		exit(-1);
	}
    int reuse=1;
	if(-1==setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,(const char*)&reuse,sizeof(int))){
		perror("setsockopt");
		exit(-1);
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(port);
	server_addr.sin_addr.s_addr=inet_addr(ip);

	if(-1==bind(sfd,(struct sockaddr*)&server_addr,sizeof(server_addr))){
		perror("bind");
		exit(-1);
	}

	return sfd;
}


int tcp_accept(int sfd){
	struct sockaddr_in client_addr;
	int addrlen=sizeof(client_addr);

	if(-1==listen(sfd,10)){
		perror("listen");
		exit(-1);
	}

    int newfd=accept(sfd,(struct sockaddr*)&client_addr,&addrlen);
	if(newfd==-1){
		perror("accept");
		exit(-1);
	}
	return newfd;
}

int tcp_connect(const char* ip,int port){
	int sfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd==-1){
		perror("socket");
		exit(-1);
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(port);
	server_addr.sin_addr.s_addr=inet_addr(ip);

	if(-1==connect(sfd,(struct sockaddr*)&server_addr,sizeof(server_addr))){
		perror("connect");
		exit(-1);
	}
	return sfd;
}


void trim_space(char *s){
	char *pstr=s;
	while(1){
		while(*s==' ' && *s!='\0') s++;
		if(*s=='\0') break;

		while(*s!=' ' && *s!='\0'){
			*pstr=*s;
			pstr++;
			s++;
		}

		if(*s=='\0') break;
		*pstr=' ';
		pstr++;
	}

	*pstr='\0';
}

void fun_ls(int client_fd){
     DIR *pDir;
	 struct dirent* pDirInfo;
	 struct stat statbuf;

	 pDir=opendir(".");
	 char buf[100];
	 MSG msg;
	 while((pDirInfo=readdir(pDir))!=NULL){
		 memset(buf,0,sizeof(buf));
		 memset(&msg,0,sizeof(MSG));
		 if(strcmp(pDirInfo->d_name,".")==0 || strcmp(pDirInfo->d_name,"..")==0){
			 continue;
		 }

		 strcpy(msg.s_msg,pDirInfo->d_name);
		 lstat(pDirInfo->d_name,&statbuf);
      
		 if(S_ISDIR(statbuf.st_mode)){
            sprintf(buf," %15dB\tdirectory\n",(int)statbuf.st_size);
		 }
		 else{
			 sprintf(buf," %15dB\t file\n",(int)statbuf.st_size);
		 }
         
		 strcat(msg.s_msg,buf);
		 msg.s_len=strlen(msg.s_msg);

		 send(client_fd,&msg,msg.s_len+MSG_LEN,0);
	 }
}

void download(int sfd,const char* filename){
	 MSG msg;
	 int fd=open(filename,O_RDONLY);
	 if(fd==-1){
		 printf("open file faiure!\n");
		 exit(-1);
	 }

	 memset(&msg,0,sizeof(MSG));
	 strcpy(msg.s_msg,filename);
	
	 msg.s_len=strlen(filename);
	 send(sfd,&msg,MSG_LEN+msg.s_len,0);


	 int read_n;
	 while(memset(&msg,0,sizeof(MSG)),(read_n=read(fd,msg.s_msg,100))>0){
		 msg.s_len=read_n;
		 send(sfd,&msg,MSG_LEN+msg.s_len,0);
	 }
	 msg.s_len=0;
	 send(sfd,&msg,msg.s_len+MSG_LEN,0);

	 close(fd);
}


void upload(int sfd,const char* filename){
     int fd;
	 fd=open(filename,O_WRONLY|O_CREAT,0666);
	 if(fd==-1){
		 printf("creat file %s failure!\n",filename);
		 exit(-1);
	 }

	 MSG msg;
	 int read_n;
	 while(memset(&msg,0,sizeof(MSG)),recv(sfd,&msg,MSG_LEN,0)>0){
          if(msg.s_len==0) break;

		  int total=0,read_n=0;
		  while(total<msg.s_len){
			  memset(msg.s_msg,0,sizeof(msg.s_msg));
			  read_n=recv(sfd,msg.s_msg,msg.s_len-total,0);
			  total+=read_n;
			  write(fd,msg.s_msg,read_n);
		  }
	 }
	 close(fd);
}

void fun_cd(int client_fd,char *dirname){
     chdir(dirname);
	 fun_ls(client_fd);
}

