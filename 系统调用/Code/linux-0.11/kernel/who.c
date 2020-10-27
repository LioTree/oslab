/*
 *  linux/kernel/who.c
 *
 *  (C) 2020 LionTree
 */

/*
 * whoami?
 */

#include <linux/kernel.h>
#include <errno.h>
#include <asm/segment.h>
#include <string.h>

#define MAXLEN 23

char username[MAXLEN]="";

extern int printk(const char *fmt, ...);

int strlen_fs(char * name)
{
	int i=0;
	while(get_fs_byte(name+i) != '\0')
	{
		i++;
	}
	return i;
}

int sys_iam(const char * name)
{
	int i,length;

	length = strlen_fs(username);
	if(length>MAXLEN)
	{
		printk("Your name is too long! MAXLEN is %d\n",MAXLEN);
		printk("The length of name is %d\n",length);
		errno = EINVAL;
		return -1;
	}

	for(i=0;(username[i] = get_fs_byte(name+i)) != '\0';i++)
	{
	}
	printk("Write name \"%s\" successfully!\n",username);
	return length;
}

int sys_whoami(char * name,unsigned int size)
{
	int i,length;

	length = strlen(username);
	if(length > size)
	{
		printk("The length of name is %d\n",length);
		printk("But your size is only %d\n",size);
		errno = EINVAL;
		return -1;
	}
	
	for(i=0;username[i] != '\0';i++)
	{
		put_fs_byte(username[i],name+i);
	}
	printk("Copy username \"%s\"  successfully!\n",username);
	return length;
}
