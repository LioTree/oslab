#include <linux/sem.h>
#include <linux/sched.h>
#include <unistd.h>
#include <asm/segment.h>
#include <linux/tty.h>
#include <linux/kernel.h>
#include <linux/fdreg.h>
#include <asm/system.h>
#include <asm/io.h>

sem_t semtables[SEM_TABLES_LEN];
int top = 1;

sem_t *sys_sem_open(const char *name, unsigned int value)
{
    // printk("enter sys_sem_open\n");
    // char temp_name[SEM_NAME_LEN] = "";
    char temp_name[SEM_NAME_LEN];
    int i, empty = -1;

    for (i = 0; (temp_name[i] = get_fs_byte(name + i)) != '\0'; i++)
    {
    }

    for (i = 0; i < top; i++)
    {
        if (strcmp(temp_name, semtables[i].name) == 0)
        {
            return &(semtables[i]);
        }

        if (semtables[i].name[0] == '\0') //寻找top前的空位
        {
            empty = i;
        }
    }
    if (empty == -1) //top前无空位，使用top标记的位置
    {
        empty = top;
        top++;
    }
    strcpy(semtables[empty].name, temp_name);
    semtables[empty].value = value;
    // semtables[empty].queue = current;
    // printk("leave sys_sem_open\n");
    return &(semtables[empty]);
}

int sys_sem_wait(sem_t *sem)
{
    // printk("enter sys_sem_wait\n");
    cli();
    while (sem->value <= 0)
    {
        sleep_on(&(sem->queue));
    }
    sem->value--;
    sti();
    // printk("leave sys_sem_wait\n");
    return 0;
}

int sys_sem_post(sem_t *sem)
{
    // printk("enter sys_sem_post\n");
    cli();
    sem->value++;
    if (sem->value <= 1)
    {
        wake_up(&(sem->queue));
    }
    sti();
    // printk("leave sys_sem_post\n");
    return 0;
}

int sys_sem_unlink(const char *name)
{
    // printk("enter sys_sem_unlink\n");
    // char temp_name[SEM_NAME_LEN] = "";
    char temp_name[SEM_NAME_LEN];
    int i;

    for (i = 0; (temp_name[i] = get_fs_byte(name + i)) != '\0'; i++)
    {
    }

    for (i = 0; i < top; i++)
    {
        if (strcmp(temp_name, semtables[i].name) == 0)
        {
            semtables[i].name[0] = '\0';
            return 0;
        }
    }
    // printk("leave sys_sem_unlink\n");
    return -1;
}