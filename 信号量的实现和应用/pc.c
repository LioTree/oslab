#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 10
#define N 3
#define M 500

void unlink_sem()
{
    sem_unlink("empty");
    sem_unlink("full");
    sem_unlink("mutex");
}

int main()
{
    int status, i;
    FILE *fp;
    sem_t *sem_full = NULL;
    sem_t *sem_empty = NULL;
    sem_t *sem_mutex = NULL;
    char buffer[M + 10][20] = {0};
    int temp;

    atexit(unlink_sem);
    fp = fopen("buffer.txt", "w"); //若buffer.txt已存在，将其删除
    fp = fopen("buffer.txt", "a");
    fflush(fp);

    unlink_sem();
    sem_full = sem_open("full", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
    sem_empty = sem_open("empty", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, BUFFER_SIZE);
    sem_mutex = sem_open("mutex", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);

    if (sem_full == SEM_FAILED || sem_empty == SEM_FAILED || sem_mutex == SEM_FAILED)
    {
        perror("sem_open");
        return -1;
    }

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
            fclose(fp);
            fp = fopen("buffer.txt", "w");
            fseek(fp, 0, SEEK_SET);
            for (int j = 1; j < i - 1; j++)
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
    sem_close(sem_full);
    sem_close(sem_empty);
    sem_close(sem_mutex);

    return 0;
}