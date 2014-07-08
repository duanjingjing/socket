/*************************************************************************
	> File Name: ftp_server.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Fri 04 Jul 2014 07:07:37 PM PDT
 ************************************************************************/

#include "ftp.h"

void handle(int signo){
     wait(NULL);
	 printf("回收子进程资源....\n");
}

int main(int argc,char *argv[]){
	if(argc!=3){
		printf("USAGE:EXE IP PORT");
		exit(-1);
	}
    
	int server_fd,rcv_n,client_fd;
    char buf[1024],tmp[100];
	server_fd=tcp_init(argv[1],atoi(argv[2]));
    MSG msg;
   
	while(1){
	    memset(buf,0,1024);
        memset(&msg,0,sizeof(MSG));
	    client_fd=tcp_accept(server_fd);
        printf("someone connect....\n");
		if(fork()==0){
			while(1){
				recv(client_fd,buf,1024,0);
		//		printf("%s\n",buf);
                if(strncmp(buf,"bye",3)==0) break;
                        
				if(strcmp(buf,"ls")==0){
					fun_ls(client_fd);
					memset(&msg,0,sizeof(MSG));
				    msg.s_len=0;
					send(client_fd,&msg,MSG_LEN+msg.s_len,0);
				}

				else if(strncmp(buf,"cd",2)==0){
					char *p=buf;
					p+=3;

					char dirent[20];
					strcpy(dirent,p);
				    fun_cd(dirent);
					msg.s_len=0;
					send(client_fd,&msg,MSG_LEN+msg.s_len,0);
				 }
       
				else if(strncmp(buf,"gets",4)==0){
					char* p=buf;
					p+=5;
					char filename[20];
					strcpy(filename,p);
				//	printf("%s\n",filename);
					download(client_fd,filename);
				}

				else if(strncmp(buf,"puts",4)==0){
					MSG msg;
					 char filename[20];
					 memset(&msg,0,sizeof(MSG));
   
					 char* p=buf;
					 p+=5;
	      
					 strcpy(filename,p);
				//	 strcat(filename,"s");
					// printf("recv filename %s !\n",filename);
					 upload(client_fd,filename);
				}
			}
			break;
		}

		else{
           signal(17,handle);
           close(client_fd);
		}
	}
	close(server_fd);
   
}


