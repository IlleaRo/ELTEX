//
// Created by illearo on 7/20/23.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int fd_fifo; /*дескриптор FIFO*/
    int randNum;

    /*Открываем fifo для чтения*/
    if ((fd_fifo = open("/tmp/fifo0001.1", O_RDONLY)) == -1)
    {
        fprintf(stderr, "Невозможно открыть fifo\n");
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));
    while (1)
    {
        read(fd_fifo, &randNum, sizeof(int));
        printf("%d\n", randNum);
    }
    close(fd_fifo);
    exit(EXIT_SUCCESS);
}