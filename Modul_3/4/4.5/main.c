#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#define SEM_NAME "/my_sem"

int main(int argc, char* argv[]) {
    int numOfRand;
    char *endPtr;
    int pFile;
    unlink("file.txt");
    sem_t* semid;
    if ((semid = sem_open(SEM_NAME, O_CREAT | O_RDWR, 0666, 1)) == SEM_FAILED) {
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
    switch (pid = fork()) {
        case -1:
            perror("problem with fork!\n");
            close(myPipe[0]);
            exit(EXIT_FAILURE);
        case 0:
            srand(time(NULL));
            close(myPipe[0]);
            int sentValue;
            for (int i = 0; i < numOfRand; ++i) {
                sentValue = rand() % 100;
                write(myPipe[1], &sentValue, sizeof(int));

                sleep(1);

                sem_wait(semid);
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
                sem_post(semid);
                printf("\n");
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
                sem_wait(semid);
                if ((pFile = open("file.txt", O_WRONLY | O_CREAT | O_APPEND, 0600)) == -1) {
                    perror("problem with file!\n");
                    exit(EXIT_FAILURE);
                }
                write(pFile, &readRand, sizeof(int));
                close(pFile);
                sem_post(semid);
            }
            close(myPipe[0]);

            break;
    }
    sleep(3);
    sem_wait(semid);
    sem_unlink(SEM_NAME);
    exit(EXIT_SUCCESS);
}