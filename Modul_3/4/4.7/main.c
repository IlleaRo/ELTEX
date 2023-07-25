#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <time.h>
#include <signal.h>
#include <sys/sem.h>
#include <limits.h>

#define WORK_FILE "/etc/fstab"
#define PROJ_ID 1
#define MAX_SEQUENCE_LEN 10


void SIGINT_exit(int);

int counter = 0;

typedef union semun {
    int val; /* значение для SETVAL */
    struct semid_ds *buf; /* буферы для IPC_STAT, IPC_SET */
    unsigned short *array; /* массивы для GETALL, SETALL */
/* часть, особенная для Linux: */
    struct seminfo *__buf; /* буфер для IPC_INFO */
} semun;

int main(int argc, char* argv[]) {

    key_t keySem; // идентификатор будущего семафора
    int semid;
    if ((keySem = ftok("/tmp", PROJ_ID)) == -1)
    {
        perror("ftok semaphor:");
        exit(EXIT_FAILURE);
    }
    if ((semid = semget(keySem, 1, 0666 | IPC_CREAT)) == -1)
    {
        perror("semget sem:");
        exit(EXIT_FAILURE);
    }
    semun arg;
    arg.val = 0;
    semctl(semid,0,SETVAL,arg);
    struct sembuf timeToWritePart = {0, 0, SEM_UNDO};
    struct sembuf endWritePart = {0, 2, SEM_UNDO};
    struct sembuf timeToReadPart = {0, -1, SEM_UNDO};
    struct sembuf endReadPart = {0, -1, SEM_UNDO};

    key_t key;
    int shmID;
    int* shmaAddr;
    if ((key = ftok(WORK_FILE, PROJ_ID)) == -1) {
        perror("FTOK");
        exit(EXIT_FAILURE);
    }
    int numberOfGen;
    if (argc < 2)
        numberOfGen = 1;
    else {
        char* endPtr;
        numberOfGen = strtol(argv[1], &endPtr, 10);
    }
    printf("Program will generate %d number sets!\n", numberOfGen);

    if ((shmID = shmget(key,sizeof(int)*(MAX_SEQUENCE_LEN + 3),
                        IPC_CREAT | 0666)) == -1)
    {
        perror("SHMGET");
        exit(EXIT_FAILURE);
    }
    if ((shmaAddr = shmat(shmID,NULL,0)) == (void *) -1)
    {
        perror("SHMAT");
        exit(EXIT_FAILURE);
    }
    pid_t forkPid;
    int seqSize;
    switch ((forkPid = fork())) {
        case -1:
            perror("FORK");
            exit(EXIT_FAILURE);
        case 0:
            signal(SIGINT,SIGINT_exit);
            int max = INT_MIN;
            int min = INT_MAX;
            while (1) {
                semop(semid, &timeToReadPart, 1);
                seqSize = shmaAddr[0];
                for (int i = 1; i < seqSize+1; ++i) {
                    if (max < shmaAddr[i])
                        max = shmaAddr[i];
                    if (min > shmaAddr[i])
                        min = shmaAddr[i];
                }
                shmaAddr[MAX_SEQUENCE_LEN-2] = min;
                shmaAddr[MAX_SEQUENCE_LEN-1] = max;
                semop(semid, &endReadPart, 1);
                min = INT_MAX;
                max = INT_MIN;
                counter++;
            }
        default:
            srand(time(NULL));
            for (int i = 0; i < numberOfGen; ++i) {
                seqSize = rand() % MAX_SEQUENCE_LEN+1;
                semop(semid, &timeToWritePart, 1);
                shmaAddr[0] = seqSize;
                int rNum;
                printf("RANDGEN (%d): ", seqSize);
                for (int j = 1; j < seqSize+1; ++j) {
                    rNum = rand() % 100;
                    printf("%d ", rNum);
                    shmaAddr[j] = rNum;
                }
                semop(semid, &endWritePart, 1);
                semop(semid, &timeToWritePart, 1);
                printf("\nMIN: %d\n", shmaAddr[MAX_SEQUENCE_LEN-2]);
                printf("MAX: %d\n", shmaAddr[MAX_SEQUENCE_LEN-1]);
            }
            shmdt(shmaAddr);
            kill(forkPid,SIGINT);
    }
    shmctl(shmID,IPC_RMID,NULL);
    exit(EXIT_SUCCESS);
}

void SIGINT_exit(int a){
    printf("Was processed %d sets\n", counter);
    exit(EXIT_SUCCESS);
}