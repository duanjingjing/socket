/*************************************************************************
	> File Name: test.c
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Wed 27 Aug 2014 08:51:27 PM PDT
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>

int main(){
	char a[100];
	int b;

	while(1){
		printf("Please Enter the messag:");
		fgets(a,100,stdin);

		printf("Please enter a integer:");
		scanf("%d",&b);
		printf("%s",a);
		printf("%d\n",b);
	}

	return 0;
}
