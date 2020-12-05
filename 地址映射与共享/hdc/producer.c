#define __LIBRARY__
#include <stdio.h>
#include "unistd.h"
#include "sem.h"
#include <string.h>

_syscall2(int, sem_open,const char*, name,unsigned int, value);
_syscall1(int,sem_wait,sem_t *,sem);
_syscall1(int,sem_post,sem_t *,sem);
_syscall1(int,sem_unlink,const char*,name);
_syscall1(int,shmget,int,key);
_syscall1(void*,shmat,int,shmid);

#define NUMBER 520 /*打出数字总数*/
#define BUFSIZE 10 /*缓冲区大小*/

sem_t   *empty, *full, *mutex;

int main()
{
   int  i,shmid;
    int *p;
    int  buf_in = 0; /*写入缓冲区位置*/
    /*打开信号量*/
    mutex = sem_open("carpelamutex",1);
    empty = sem_open("carpelaempty",10);
    full = sem_open("carpelafull",0);

    shmid = shmget(1);
    p = (int*) shmat(shmid);
    /*生产者进程*/
    for( i = 0 ; i < NUMBER; i++)
    {
        sem_wait(empty);
        sem_wait(mutex);
        p[buf_in] = i;
        buf_in = ( buf_in + 1)% BUFSIZE;
        sem_post(mutex);
        sem_post(full);
    }
    /*释放信号量*/
    sem_unlink("carpelafull");
    sem_unlink("carpelaempty");
    sem_unlink("carpelamutex");
    return 0; 
}
