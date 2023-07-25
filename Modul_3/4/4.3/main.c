#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define PROJ_ID 2
#define MAX_TO_READ 3
#define FORK_NUM 5

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
    arg.val = MAX_TO_READ;
    semctl(semid, 0, SETVAL, arg);
    arg.val = FORK_NUM;
    semctl(semid, 1, SETVAL, arg);

    struct sembuf lock_to_write[2] = {{0, -MAX_TO_READ, SEM_UNDO},
                             {0, 0, SEM_UNDO}};
    struct sembuf unlock_from_write = {0, MAX_TO_READ, SEM_UNDO};
    struct sembuf lock_to_read = {0, -1, SEM_UNDO};
    struct sembuf unlock_from_read = {0, 1, SEM_UNDO};

    struct sembuf readers_lock = {1,-1,SEM_UNDO};
    struct sembuf readers_unlock = {1,1,SEM_UNDO};
    struct sembuf finish_lock = {1, -FORK_NUM, SEM_UNDO};



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


    for (int i = 1; i < FORK_NUM; ++i)
        if (!(pid = fork())) {
            semop(semid,&readers_lock,1);
            close(myPipe[0]);
            close(myPipe[1]);
            for (int j = 0; j < 4; ++j) {

                srand(time(NULL)+getpid());
                sleep(rand() % 10);
                semop(semid, &lock_to_read, 1);
                if ((pFile = open("file.txt", O_RDONLY)) == -1) {
                    printf("open error (%d): file doesn't exist!\n", i);
                    continue;
                }
                printf("FILE (READER %d): ", i);
                int readNum;
                while (1) {
                    if (read(pFile, &readNum, sizeof(int)) != sizeof(int))
                        break;
                    printf("%d ", readNum);
                }
                close(pFile);
                semop(semid, &unlock_from_read, 1);
                printf("\n");
            }
            semop(semid,&readers_unlock,1);
            exit(EXIT_SUCCESS);
        }

    switch (pid = fork()) {
        case -1:
            perror("problem with fork!\n");
            close(myPipe[0]);
            exit(EXIT_FAILURE);
        case 0:
            semop(semid,&readers_lock,1);
            srand(time(NULL));
            close(myPipe[0]);
            int sentValue;
            for (int i = 0; i < numOfRand; ++i) {
                sentValue = rand() % 100;
                write(myPipe[1], &sentValue, sizeof(int));
                sleep(4);
                semop(semid, &lock_to_read, 1);
                if ((pFile = open("file.txt", O_RDONLY)) == -1) {
                    perror("open (0):");
                    exit(EXIT_FAILURE);
                }

                printf("FILE (READER 0): ");
                for (int j = 0; j < i + 1; ++j) {
                    if (read(pFile, &sentValue, sizeof(int)) != sizeof(int))
                        break;
                    printf("%d ", sentValue);
                }
                close(pFile);
                semop(semid, &unlock_from_read, 1);

                printf("\n");
            }
            semop(semid,&readers_unlock,1);

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
                semop(semid, lock_to_write, 2);
                if ((pFile = open("file.txt", O_WRONLY | O_APPEND | O_CREAT, 0600)) == -1) {
                    perror("problem with file!\n");
                    exit(EXIT_FAILURE);
                }
                write(pFile, &readRand, sizeof(int));
                close(pFile);
                semop(semid, &unlock_from_write, 1);
            }
            close(myPipe[0]);
            break;
    }
    semop(semid,&finish_lock,1);
    sleep(3);
    semctl(semid, 0, IPC_RMID);
    exit(EXIT_SUCCESS);
}