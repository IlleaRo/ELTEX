#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int numOfRand;
    char* endPtr;
    if (argc < 2)
    {
        printf("Program will generate 1 number!\n");
        numOfRand = 1;
    }
    else
        numOfRand = (int) strtol(argv[1],&endPtr,10);
    int pt;
    int myPipe[2];
    if ((pipe(myPipe)) == -1)
    {
        perror("problem with pipe!\n");
        exit(EXIT_FAILURE);
    }
    switch (fork())
    {
        case -1:
            perror("problem with fork!\n");
            close(myPipe[0]);
            exit(EXIT_FAILURE);
        case 0:
            srand(time(NULL));
            close(myPipe[0]);
            int sentValue;
            for (int i = 0; i < numOfRand; ++i)
            {
                sentValue = rand();
                write(myPipe[1],&sentValue, sizeof(int));
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
            for (int i = 0; i < numOfRand; ++i) {
                read(myPipe[0],&readRand,sizeof(int));
                printf("%-2d\t%d\n",i+1,readRand);
            }
            close(myPipe[0]);
            break;
    }
    exit(EXIT_SUCCESS);
}
