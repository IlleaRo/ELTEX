#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/sem.h>
#include <limits.h>
#include <sys/mman.h>
#include <fcntl.h>           /* For O_* constants */



#define WORK_FILE "/etc/fstab"
#define PROJ_ID 1
#define MAX_SEQUENCE_LEN 10
#define SHAREDM_NAME "/my_shM"


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



    int numberOfGen;
    if (argc < 2)
        numberOfGen = 1;
    else {
        char* endPtr;
        numberOfGen = strtol(argv[1], &endPtr, 10);
    }
    printf("Program will generate %d number sets!\n", numberOfGen);
    int shmID;
    if ((shmID = shm_open(SHAREDM_NAME, O_CREAT | O_RDWR, 0666)) == -1)
    {
        perror("SHMGET");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shmID,(__off_t) sizeof(int) * (numberOfGen + 2)) == -1)
    {
        perror("TRUNCATE");
        exit(EXIT_FAILURE);
    }

    int* shmAddr;
    if ((shmAddr = mmap(NULL,sizeof(int) * (numberOfGen + 2),
                        PROT_READ | PROT_WRITE, MAP_SHARED_VALIDATE,
                        shmID,0)) == MAP_FAILED)
    {
        perror("MMAP");
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
            if ((shmAddr = mmap(NULL,sizeof(int) * (numberOfGen + 2),
                                PROT_READ | PROT_WRITE, MAP_SHARED_VALIDATE,
                                shmID,0)) == MAP_FAILED)
            {
                perror("MMAP");
                exit(EXIT_FAILURE);
            }
            while (1) {
                semop(semid, &timeToReadPart, 1);
                seqSize = shmAddr[0];
                for (int i = 1; i < seqSize+1; ++i) {
                    if (max < shmAddr[i])
                        max = shmAddr[i];
                    if (min > shmAddr[i])
                        min = shmAddr[i];
                }
                shmAddr[MAX_SEQUENCE_LEN-2] = min;
                shmAddr[MAX_SEQUENCE_LEN-1] = max;
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
                shmAddr[0] = seqSize;
                int rNum;
                printf("RANDGEN (%d): ", seqSize);
                for (int j = 1; j < seqSize+1; ++j) {
                    rNum = rand() % 100;
                    printf("%d ", rNum);
                    shmAddr[j] = rNum;
                }
                semop(semid, &endWritePart, 1);
                semop(semid, &timeToWritePart, 1);
                printf("\nMIN: %d\n", shmAddr[MAX_SEQUENCE_LEN-2]);
                printf("MAX: %d\n", shmAddr[MAX_SEQUENCE_LEN-1]);
            }
            munmap(shmAddr, sizeof(int) * (numberOfGen + 2));
            kill(forkPid,SIGINT);
    }
    shm_unlink(SHAREDM_NAME);
    exit(EXIT_SUCCESS);
}

void SIGINT_exit(int a){
    printf("Was processed %d sets\n", counter);
    exit(EXIT_SUCCESS);
}