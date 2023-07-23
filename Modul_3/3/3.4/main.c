#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define SIZE_OF_BUFFER 20

void myHandler(int);

int main() {

    signal(SIGINT,myHandler);

    int pFile;
    int counter = 0;
    unlink("file.txt");
    char buffer[SIZE_OF_BUFFER];
    while (1)
    {
        sprintf(buffer,"%d", ++counter);
        buffer[strlen(buffer)] = '\n';
        if ((pFile = open("file.txt", O_CREAT | O_APPEND | O_WRONLY,0600)) == -1)
        {
            perror("Problem with file!\n");
            exit(EXIT_FAILURE);
        }
        write(pFile,buffer,strlen(buffer));
        close(pFile);
        sleep(1);
    }
}

void myHandler(int t)
{
    printf("SIGINT have got!\n");
}