/*************************************************************************
	> File Name: msg.h
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 02 Jul 2014 06:57:42 PM PDT
 ************************************************************************/
#ifndef _MSG_H_
#define _MSG_H_

#define MSG_SIZE 8188
#define MSG_LEN (8192-MSG_SIZE)
typedef struct tag{
	int s_len;
	char s_buf[MSG_SIZE];
}MSG;

#endif


