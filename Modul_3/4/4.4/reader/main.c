#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#define SEM_NAME "/my_sem_pipe"
#define PIPE_NAME "/tmp/fifo0001.1"

int main() {
    sem_t* sem_for_pipe;
    if ((sem_for_pipe = sem_open(SEM_NAME, O_RDWR)) == SEM_FAILED)
    {
        perror("SEM_OPEN");
        exit(EXIT_FAILURE);
    }
    int fd_fifo; /*дескриптор FIFO*/
    if ((fd_fifo = open(PIPE_NAME, O_RDONLY | O_NONBLOCK)) == -1)
    {
        fprintf(stderr, "Невозможно открыть fifo\n");
        exit(EXIT_FAILURE);
    }
    int j, num;
    int sem_value;
    sem_getvalue(sem_for_pipe, &sem_value);
    printf("Reader: Semaphore value before sem_wait: %d\n", sem_value);
    sem_wait(sem_for_pipe);
    read(fd_fifo, &j, sizeof(int));
    printf("Will read %d numbers\n", j);
    for (int i = 0; i < j; ++i)
    {
        read(fd_fifo, &num, sizeof(int));
        printf("%d\n", num);
    }
    sem_post(sem_for_pipe);
    close(fd_fifo);
    sem_close(sem_for_pipe);
    exit(EXIT_SUCCESS);
}
