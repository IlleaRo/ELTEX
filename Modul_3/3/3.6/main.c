#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>



void sig2Handler(int); // Обработчик сигналов SIGUSR1

int main(int argc, char* argv[]) {
    int numOfRand;
    char* endPtr;
    int pFile;
    unlink("file.txt");
    if (argc < 2)
    {
        printf("Program will generate 1 number!\n");
        numOfRand = 1;
    }
    else
        numOfRand = (int) strtol(argv[1],&endPtr,10);
    int pid;
    int myPipe[2];
    if ((pipe(myPipe)) == -1)
    {
        perror("problem with pipe!\n");
        exit(EXIT_FAILURE);
    }
    switch (pid = fork())
    {
        case -1:
            perror("problem with fork!\n");
            close(myPipe[0]);
            exit(EXIT_FAILURE);
        case 0:
            signal(SIGUSR2,sig2Handler);

            srand(time(NULL));
            close(myPipe[0]);
            int sentValue;
            for (int i = 0; i < numOfRand; ++i)
            {
                sentValue = rand()%100;
                write(myPipe[1],&sentValue, sizeof(int));
                pause();
                if ((pFile = open("file.txt", O_RDONLY)) != -1) {
                    printf("FILE: ");
                    for (int j = 0; j < i+1; ++j) {
                        if (read(pFile, &sentValue, sizeof(int)) != sizeof(int))
                            break;
                        printf("%d ", sentValue);
                    }
                    close(pFile);
                    printf("\n");
                } else
                {
                    printf("CHILD: Can't work with file!\n");
                }
            }
            exit(EXIT_SUCCESS);
        default:
            if (dup2(myPipe[0],myPipe[1]) == -1)
            {
                perror("problem with dup2\n!");
                exit(EXIT_FAILURE);
            }
            close(myPipe[1]);
            int readRand;
            

            for (int i = 0; i < numOfRand; ++i)
            {
                read(myPipe[0],&readRand,sizeof(int));
                printf("%-2d\t%d\n",i+1,readRand);
                if ((pFile = open("file.txt",  O_WRONLY | O_CREAT | O_APPEND,0600)) == -1)
                {
                    perror("problem with file!\n");
                    exit(EXIT_FAILURE);
                }
                write(pFile,&readRand, sizeof(int));
                close(pFile);
                kill(pid,SIGUSR2);
            }
            close(myPipe[0]);

            break;
    }
    exit(EXIT_SUCCESS);
}

void sig2Handler(int)
{
    printf("CHILD: file permission granted!\n");
}