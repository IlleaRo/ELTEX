#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int numOfRand;
    char* endPtr;
    int readNum;
    int sendNum;
    if (argc < 2)
    {
        printf("Program will generate 1 number!\n");
        numOfRand = 1;
    }
    else
        numOfRand = (int) strtol(argv[1],&endPtr,10);
    int pt;
    int pipe_1[2], pipe_2[2];
    if ((pipe(pipe_1)) == -1 || (pipe(pipe_2)) == -1)
    {
        perror("problem with pipe!\n");
        exit(EXIT_FAILURE);
    }
    switch (fork())
    {
        case -1:
            perror("problem with fork!\n");
            close(pipe_1[0]);
            exit(EXIT_FAILURE);
        case 0:
            srand(time(NULL));
            close(pipe_1[0]);
            if (dup2(pipe_2[0], pipe_2[1]) == -1)
            {
                perror("problem with dup2\n!");
                exit(EXIT_FAILURE);
            }
            close(pipe_2[1]);
            for (int i = 0; i < numOfRand; ++i)
            {
                sendNum = rand()%100;
                write(pipe_1[1], &sendNum, sizeof(int));
                read(pipe_2[0],&readNum,sizeof(int));
                printf("CHILD get message: %d\n", readNum);
            }
            exit(EXIT_SUCCESS);
        default:
            close(pipe_2[0]);
            if (dup2(pipe_1[0], pipe_1[1]) == -1)
            {
                perror("problem with dup2\n!");
                exit(EXIT_FAILURE);
            }
            close(pipe_1[1]);
            for (int i = 0; i < numOfRand; ++i) {
                read(pipe_1[0], &readNum, sizeof(int));
                printf("PARENT get message: %d\n",readNum);
                readNum*=2;
                write(pipe_2[1], &readNum, sizeof(int));
            }
            break;
    }
    exit(EXIT_SUCCESS);
}
