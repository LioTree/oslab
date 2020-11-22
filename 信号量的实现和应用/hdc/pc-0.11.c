#define __LIBRARY__
#include <stdio.h>
#include "unistd.h"
#include "sem.h"
#include <string.h>

#define BUFFER_SIZE 10
#define N 3
#define M 500

_syscall2(int, sem_open,const char*, name,unsigned int, value);
_syscall1(int,sem_wait,sem_t *,sem);
_syscall1(int,sem_post,sem_t *,sem);
_syscall1(int,sem_unlink,const char*,name);

void unlink_sem()
{
    sem_unlink("empty");
    sem_unlink("full");
    sem_unlink("mutex");
}

int main()
{
    int status, i,j;
    FILE *fp;
    sem_t *sem_full = NULL;
    sem_t *sem_empty = NULL;
    sem_t *sem_mutex = NULL;
    char buffer[M + 10][20] = {0};
    int temp;

    fp = fopen("buffer.txt", "w");
    fp = fopen("buffer.txt", "a");
    fflush(fp);

    unlink_sem();
    sem_full = (sem_t *)sem_open("full",0);
    sem_empty = (sem_t *)sem_open("empty",  BUFFER_SIZE);
    sem_mutex = (sem_t *)sem_open("mutex", 1);

    for (i = 0; i < N; i++)
    {
        status = fork();
        if (status == 0 || status == -1)
            break; 
    }

    if (status == -1)
    {
        printf("fork error\n");
    }
    else if (status == 0) 
    {
        while (1)
        {
            sem_wait(sem_full);
            sem_wait(sem_mutex);
            fp = fopen("buffer.txt", "r+");
            fseek(fp, 0, SEEK_SET);
            for (i = 0; !feof(fp); i++)
            {
                memset(buffer[i], 0, sizeof(buffer[i]));
                fgets(buffer[i], sizeof(buffer[i]) - 1, fp); 
            }
            printf("%d:%s", getpid(), buffer[0]);
            fflush(stdout);
            fclose(fp); /*这个一定不能少，否则会general protection: 0000*/
            fp = fopen("buffer.txt", "w");
            fseek(fp, 0, SEEK_SET);

            for (j = 1; j < i - 1; j++)
            {
                fprintf(fp, "%s", buffer[j]);
            }
            fflush(fp);
            fclose(fp);
            sem_post(sem_mutex);
            sem_post(sem_empty);
        }
    }
    else
    {
        for (i = 0; i <= M; i++)
        {
            sem_wait(sem_empty);
            sem_wait(sem_mutex);
            fprintf(fp, "%d\n", i);
            fflush(fp);
            sem_post(sem_mutex);
            sem_post(sem_full);
        }
        exit(0);
        wait(NULL);
    }
    fclose(fp);
    unlink_sem();

    return 0;
}
