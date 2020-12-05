#ifndef _SEM_H
#define _SEM_H

#include<linux/sched.h>
// #include<string.h>

#define SEM_NAME_LEN 100
#define SEM_TABLES_LEN 100

typedef struct semaphore{
    char name[SEM_NAME_LEN];
    int value;
    struct task_struct * queue;
}sem_t;

#endif