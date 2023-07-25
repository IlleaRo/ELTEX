#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <time.h>
#include <fcntl.h>

#define PROJ_ID 1

int main()
{
    int fd_fifo; /*дескриптор FIFO*/
    int randNum;
    key_t key; // идентификатор будущего семафора
    int semid;
    if ((key = ftok("/tmp", PROJ_ID)) == -1)
    {
        perror("ftok semaphor:");
        exit(EXIT_FAILURE);
    }
    if ((semid = semget(key, 1, 0666)) == -1)
    {
        perror("semget sem:");
        exit(EXIT_FAILURE);
    }
    struct sembuf lock_res = {0, -1, SEM_UNDO};
    struct sembuf rel_res = {0, 1, SEM_UNDO};
    /*Открываем fifo для чтения*/
    if ((fd_fifo = open("/tmp/fifo0001.1", O_RDONLY | O_NONBLOCK)) == -1)
    {
        fprintf(stderr, "Невозможно открыть fifo\n");
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));
    int j;
    semop(semid,&lock_res,1);
    read(fd_fifo, &j, sizeof(int));
    printf("Will read %d numbers\n", j);
    for (int i = 0; i < j; ++i)
    {
        read(fd_fifo, &randNum, sizeof(int));
        printf("%d\n", randNum);
    }
    semop(semid,&rel_res,1);
    close(fd_fifo);
    exit(EXIT_SUCCESS);
}