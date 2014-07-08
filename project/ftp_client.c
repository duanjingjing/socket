/*************************************************************************
	> File Name: ftp_client.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Sun 06 Jul 2014 06:50:03 PM PDT
 ************************************************************************/

#include  "ftp.h"

int main(int argc,char *argv[]){
	if(argc!=3){
		printf("USAGE:EXE IP PORT");
		exit(-1);
	}

    int client_fd;
	client_fd=tcp_connect(argv[1],atoi(argv[2]));

	char buf[1024];
	MSG msg;
	while(1){
		memset(buf,0,sizeof(buf));
   //     system("clear");
		fgets(buf,1024,stdin);
		buf[strlen(buf)-1]='\0';
        
		trim_space(buf);
		send(client_fd,buf,strlen(buf),0);
		if(strncmp(buf,"bye",3)==0) break;
        printf("%s\n",buf);
        
		if(strcmp(buf,"ls")==0){
            while(memset(&msg,0,sizeof(MSG)),recv(client_fd,&msg,MSG_LEN,0)>0){
                if(msg.s_len==0) break;
				recv(client_fd,msg.s_msg,msg.s_len,0);
				printf("%s\n",msg.s_msg);
			}
		}

		else if(strncmp(buf,"cd",2)==0){
            while(memset(&msg,0,sizeof(MSG)),recv(client_fd,&msg,MSG_LEN,0)>0){
				if(msg.s_len==0) break;
			   recv(client_fd,msg.s_msg,msg.s_len,0);
			   printf("%s\n",msg.s_msg);
			}
		}

		else if(strncmp(buf,"gets",4)==0){
             memset(&msg,0,sizeof(MSG));
			 recv(client_fd,&msg,MSG_LEN,0);
			 recv(client_fd,msg.s_msg,msg.s_len,0);
             
		
	//		printf("recv file %s success!\n",msg.s_msg);

		//	strcat(msg.s_msg,"s");

			int fd=open(msg.s_msg,O_WRONLY|O_CREAT,0666);
			if(fd==-1){
				 printf("open file faiure!\n");
				exit(-1);
			}

			char filename[20];
			strcpy(filename,msg.s_msg);
		//	printf("open file %s success!\n",msg.s_msg);

			 while(memset(&msg,0,sizeof(MSG)),recv(client_fd,&msg,MSG_LEN,0)>0){
				 //	printf("recv msg.s_len %d\n",msg.s_len);
					if(msg.s_len==0){
						break;
					}

				int total=0,read_n=0;
				while(total<msg.s_len){
					memset(msg.s_msg,0,sizeof(msg.s_msg));
					read_n=recv(client_fd,msg.s_msg,msg.s_len-total,0);
					total+=read_n;
					write(fd,msg.s_msg,read_n);
				}
		    }
			
		    close(fd);
			printf("download ->file %s success!\n",filename);
			 
		}
        
		else if(strncmp(buf,"puts",4)==0){
             char filename[20];
			 char *p=buf;
			 p+=5;

			 strcpy(filename,p);
			 int fd=open(filename,O_RDONLY);
			 if(fd==-1){
				 printf("open file faiure!\n");
				 exit(-1);
			 }
             int read_n;
			 while(memset(&msg,0,sizeof(MSG)),(read_n=read(fd,msg.s_msg,MSG_SIZE))>0){
                 msg.s_len=read_n;
				 send(client_fd,&msg,msg.s_len+MSG_LEN,0);
			 }

			 msg.s_len=0;
			 send(client_fd,&msg,msg.s_len+MSG_LEN,0);

			 close(fd);
			 printf("upload file %s success!\n",filename);
		}
	//	getchar();
		
	}
}

