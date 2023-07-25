#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define PROJ_ID 2

typedef union semun {
    int val; /* значение для SETVAL */
    struct semid_ds *buf; /* буферы для IPC_STAT, IPC_SET */
    unsigned short *array; /* массивы для GETALL, SETALL */
/* часть, особенная для Linux: */
    struct seminfo *__buf; /* буфер для IPC_INFO */
} semun;

int main(int argc, char* argv[]) {
    int numOfRand;
    char *endPtr;
    int pFile;
    unlink("file.txt");
    key_t key; // идентификатор будущего семафора
    int semid;
    if ((key = ftok("/", PROJ_ID)) == -1) {
        perror("ftok semaphor:");
        exit(EXIT_FAILURE);
    }
    if ((semid = semget(key, 2, 0666 | IPC_CREAT)) == -1) {
        perror("semget sem:");
        exit(EXIT_FAILURE);
    }
    semun arg;
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);
    semctl(semid, 1, SETVAL, arg);
    struct sembuf lock_res = {0, -1, SEM_UNDO};
    struct sembuf rel_res = {0, 1, SEM_UNDO};
    struct sembuf lock_fork = {1, -1, SEM_UNDO};
    struct sembuf rel_fork = {1, 1, SEM_UNDO};


    if (argc < 2) {
        printf("Program will generate 1 number!\n");
        numOfRand = 1;
    } else
        numOfRand = (int) strtol(argv[1], &endPtr, 10);
    int pid;
    int myPipe[2];
    if ((pipe(myPipe)) == -1) {
        perror("problem with pipe!\n");
        exit(EXIT_FAILURE);
    }
    switch (pid = fork()) {
        case -1:
            perror("problem with fork!\n");
            close(myPipe[0]);
            exit(EXIT_FAILURE);
        case 0:
            semop(semid, &lock_fork, 1);
            srand(time(NULL));
            close(myPipe[0]);
            int sentValue;
            for (int i = 0; i < numOfRand; ++i) {
                sentValue = rand() % 100;
                write(myPipe[1], &sentValue, sizeof(int));

                sleep(1);
                semop(semid, &lock_res, 1);
                if ((pFile = open("file.txt", O_RDONLY)) == -1) {
                    perror("open:");
                    exit(EXIT_FAILURE);
                }

                printf("FILE: ");
                for (int j = 0; j < i + 1; ++j) {
                    if (read(pFile, &sentValue, sizeof(int)) != sizeof(int))
                        break;
                    printf("%d ", sentValue);
                }
                close(pFile);
                semop(semid, &rel_res, 1);

                printf("\n");

                semop(semid, &rel_fork, 1);
            }
            exit(EXIT_SUCCESS);
        default:
            if (dup2(myPipe[0], myPipe[1]) == -1) {
                perror("problem with dup2\n!");
                exit(EXIT_FAILURE);
            }
            close(myPipe[1]);
            int readRand;

            for (int i = 0; i < numOfRand; ++i) {
                read(myPipe[0], &readRand, sizeof(int));
                printf("%-2d\t%d\n", i + 1, readRand);
                semop(semid, &lock_res, 1);
                if ((pFile = open("file.txt", O_WRONLY | O_CREAT | O_APPEND, 0600)) == -1) {
                    perror("problem with file!\n");
                    exit(EXIT_FAILURE);
                }
                write(pFile, &readRand, sizeof(int));
                close(pFile);
                semop(semid, &rel_res, 1);
            }
            close(myPipe[0]);

            break;
    }
    sleep(3);
    semop(semid, &lock_fork, 1);
    semop(semid, &rel_fork, 1);
    semctl(semid, 0, IPC_RMID);
    exit(EXIT_SUCCESS);
}