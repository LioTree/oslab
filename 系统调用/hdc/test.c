#define __LIBRARY__
#include "unistd.h"
#include <stdio.h>

#define MAXLEN1 20
#define MAXLEN2 23

_syscall1(int, iam,const char*, name);
_syscall2(int, whoami,char *, name,unsigned int, size);

int main(){
	char name1[MAXLEN1];
	char name2[MAXLEN2];
	if(iam("testefghiruyejdhiuytest") != -1)
	{
		if(whoami(name1,MAXLEN1) != -1)
		{
			printf("Your name is \"%s\"\n",name1);
		}
		if(whoami(name2,MAXLEN2) != -1)
		{
			printf("Your name is \"%s\"\n",name2);
		}
	}
	return 0;
}
