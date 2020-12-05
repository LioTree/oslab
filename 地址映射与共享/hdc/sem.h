#include<linux/sched.h>

#define SEM_NAME_LEN 100

typedef struct semaphore
{
    char name[SEM_NAME_LEN];
    int value;
    struct task_struct * queue;
} sem_t;
