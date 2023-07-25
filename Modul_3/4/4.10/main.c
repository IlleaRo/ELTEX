#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <time.h>
#include <signal.h>
#include <sys/sem.h>
#include <limits.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>           /* For O_* constants */

#define WORK_FILE "/etc/fstab"
#define PROJ_ID 1
#define MAX_SEQUENCE_LEN 10

#define SHAREDM_NAME "/my_shM"

void SIGINT_exit(int);
void* searchMin(void* arg); // Возвращает указатель на минимальный элемент последовательности
void* searchSum(void* arg); // Поиск суммы элементов
void* searchMean(void* arg); // Поиск среднего значения

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

    if (ftruncate(shmID,(__off_t) sizeof(int) * (numberOfGen + 4)) == -1)
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
            pthread_t tid[3];
            void* status[3];
            if ((shmAddr = mmap(NULL,sizeof(int) * (numberOfGen + 2),
                                PROT_READ | PROT_WRITE, MAP_SHARED_VALIDATE,
                                shmID,0)) == MAP_FAILED)
            {
                perror("MMAP");
                exit(EXIT_FAILURE);
            }
            while (1) {
                semop(semid, &timeToReadPart, 1);
                pthread_create(&tid[0], NULL, searchMin, shmAddr);
                pthread_create(&tid[1], NULL, searchSum, shmAddr);
                pthread_create(&tid[2], NULL, searchMean, shmAddr);
                for (int i = 0; i < 3; ++i)
                    pthread_join(tid[i], &status[i]);
                semop(semid, &endReadPart, 1);
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
                printf("\nMIN: %d\n", shmAddr[MAX_SEQUENCE_LEN-3]);
                printf("SUM: %d\n", shmAddr[MAX_SEQUENCE_LEN-2]);
                printf("MEAN: %d\n", shmAddr[MAX_SEQUENCE_LEN-1]);
            }
            munmap(shmAddr,sizeof(int) * (numberOfGen + 4));
            kill(forkPid,SIGINT);
    }
    shm_unlink(SHAREDM_NAME);
    exit(EXIT_SUCCESS);
}

void SIGINT_exit(int a){
    printf("Was processed %d sets\n", counter);
    exit(EXIT_SUCCESS);
}

void* searchMin(void* arg){
    int min = INT_MAX;
    for (int i = 1; i < ((int*)arg)[0] + 1; ++i)
        if (min > ((int*)(arg))[i])
            min = ((int*) arg)[i];
    ((int*)arg)[MAX_SEQUENCE_LEN-3] = min;
    pthread_exit(0);
}

void* searchSum(void* arg){
    long long sum = 0;
    for (int i = 1; i < ((int*)arg)[0] + 1; ++i)
        sum+= ((int*)arg)[i];
    ((int*)arg)[MAX_SEQUENCE_LEN-2] = sum > INT_MAX? INT_MAX : (int) sum;
    pthread_exit(0);
}

void* searchMean(void* arg){
    long long sum = 0;
    for (int i = 1; i < ((int*)arg)[0] + 1; ++i)
        sum+= ((int*)arg)[i];
    long long mean = sum/((int*)arg)[0];
    ((int*)arg)[MAX_SEQUENCE_LEN-1] = mean > INT_MAX? INT_MAX : (int) mean;
    pthread_exit(0);
}