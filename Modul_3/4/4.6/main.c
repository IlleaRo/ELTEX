#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>

#define SEM_RW_NAME "/my_sem"
#define SEM_FORK_NAME "/my_sem_fork"
#define MAX_TO_READ 3
#define FORK_NUM 5

int main(int argc, char* argv[]) {
    int numOfRand;
    char *endPtr;
    int pFile;
    unlink("file.txt");

    sem_t* sem_for_rw, *sem_for_fork;

    if ((sem_for_rw = sem_open(SEM_RW_NAME, O_CREAT | O_RDWR, 0666, MAX_TO_READ)) == SEM_FAILED) {
        perror("SEM_OPEN");
        exit(EXIT_FAILURE);
    }

    if ((sem_for_fork = sem_open(SEM_FORK_NAME, O_CREAT | O_RDWR, 0666, FORK_NUM)) == SEM_FAILED) {
        perror("SEM_OPEN");
        exit(EXIT_FAILURE);
    }
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
            sem_wait(sem_for_fork);
            close(myPipe[0]);
            close(myPipe[1]);
            for (int j = 0; j < 4; ++j) {
                srand(time(NULL)+getpid());
                sleep(rand() % 10);
                sem_wait(sem_for_rw);
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
                sem_post(sem_for_rw);
                printf("\n");
            }
            sem_post(sem_for_fork);
            exit(EXIT_SUCCESS);
        }

    switch (pid = fork()) {
        case -1:
            perror("problem with fork!\n");
            close(myPipe[0]);
            exit(EXIT_FAILURE);
        case 0:
            sem_wait(sem_for_fork);
            srand(time(NULL));
            close(myPipe[0]);
            int sentValue;
            for (int i = 0; i < numOfRand; ++i) {
                sentValue = rand() % 100;
                write(myPipe[1], &sentValue, sizeof(int));
                sleep(4);
                sem_wait(sem_for_rw);
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
                sem_post(sem_for_rw);
                printf("\n");
            }
            sem_post(sem_for_fork);
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
                for (int j = 0; j < MAX_TO_READ; ++j) {
                    sem_wait(sem_for_rw);
                }
                if ((pFile = open("file.txt", O_WRONLY | O_APPEND | O_CREAT, 0600)) == -1) {
                    perror("problem with file!\n");
                    exit(EXIT_FAILURE);
                }
                write(pFile, &readRand, sizeof(int));
                close(pFile);
                for (int j = 0; j < MAX_TO_READ; ++j) {
                    sem_post(sem_for_rw);
                }
            }
            close(myPipe[0]);
            break;
    }
    sleep(3);
    for (int j = 0; j < FORK_NUM; ++j) {
        sem_wait(sem_for_fork);
    }
    sem_unlink(SEM_RW_NAME);
    sem_unlink(SEM_FORK_NAME);
    exit(EXIT_SUCCESS);
}