#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <fcntl.h>

#define PROJ_ID 1

typedef union semun {
    int val; /* значение для SETVAL */
    struct semid_ds *buf; /* буферы для IPC_STAT, IPC_SET */
    unsigned short *array; /* массивы для GETALL, SETALL */
/* часть, особенная для Linux: */
    struct seminfo *__buf; /* буфер для IPC_INFO */
} semun;

int main(int argc, char* argv[])
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
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1)
    {
        perror("semget sem:");
        exit(EXIT_FAILURE);
    }
    semun arg;
    arg.val = 1;
    semctl(semid,0,SETVAL,arg);
    struct sembuf lock_res = {0, -1, SEM_UNDO};
    struct sembuf rel_res = {0, 1, SEM_UNDO};

    /*Открываем fifo для записи*/
    if ((fd_fifo = open("/tmp/fifo0001.1", O_WRONLY | O_CREAT | O_NONBLOCK, S_IRWXU)) == -1)
    {
        fprintf(stderr, "Невозможно открыть fifo\n");
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));
    if (argc < 2)
    {
        printf("Will generate 1 number!");
        randNum = rand()%100;
        int number = 1;
        semop(semid,&lock_res,1);
        write(fd_fifo,&number,sizeof(int));
        write(fd_fifo,&randNum,sizeof(int));
        semop(semid,&rel_res,1);
    }
    else
    {
        char* endPtr;
        int number = (int) strtoq(argv[1],&endPtr,10);
        printf("Will generate %d numbers\n", number);
        semop(semid,&lock_res,1);
        write(fd_fifo,&number,sizeof(int));
        for (int i = 0; i < number; ++i) {
            randNum = rand()%100;
            printf("%d\n", randNum);
            write(fd_fifo,&randNum,sizeof(int));
            sleep(5);
        }
        semop(semid,&rel_res,1);
    }
    semop(semid,&lock_res,1);
    close(fd_fifo);
    unlink("/tmp/fifo0001.1");
    semop(semid,&rel_res,1);
    semctl(semid, 0, IPC_RMID);
    exit(EXIT_SUCCESS);
}