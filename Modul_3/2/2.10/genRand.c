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
    /*unlink("/tmp/fifo0001.1");

    if((mkfifo("/tmp/fifo0001.1", 0666)) == -1)
    {
        fprintf(stderr, "Невозможно создать fifo\n");
        exit(0);
    }*/

    /*Открываем fifo для записи*/
    if ((fd_fifo = open("/tmp/fifo0001.1", O_WRONLY, S_IRWXU)) == -1)
    {
        fprintf(stderr, "Невозможно открыть fifo\n");
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));
    while(1)
    {
        randNum = rand()%100;
        write(fd_fifo,&randNum,sizeof(int));
        sleep(5);
    }
    close(fd_fifo);
    exit(EXIT_SUCCESS);
}