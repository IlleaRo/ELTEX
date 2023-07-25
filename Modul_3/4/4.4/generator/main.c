#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define SEM_NAME "/my_sem_pipe"
#define PIPE_NAME "/tmp/fifo0001.1"


int main(int argc, char* argv[]) {
    sem_t* sem_for_pipe;
    if ((sem_for_pipe = sem_open(SEM_NAME, O_CREAT | O_RDWR, 0666, 1)) == SEM_FAILED)
    {
        perror("SEM_OPEN");
        exit(EXIT_FAILURE);
    }
    int fd_fifo; /*дескриптор FIFO*/
    /*Открываем fifo для записи*/
    if ((fd_fifo = open(PIPE_NAME, O_WRONLY | O_CREAT | O_NONBLOCK, S_IRWXU)) == -1)
    {
        fprintf(stderr, "Невозможно открыть fifo\n");
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));
    int num;
    if (argc < 2)
    {
        printf("Will generate 1 number!");
        num = rand()%100;
        int number = 1;
        sem_wait(sem_for_pipe);
        write(fd_fifo,&number,sizeof(int));
        write(fd_fifo,&num,sizeof(int));
        sem_post(sem_for_pipe);
    }
    else
    {
        char* endPtr;
        int number = (int) strtoq(argv[1],&endPtr,10);
        printf("Will generate %d numbers\n", number);
        sem_wait(sem_for_pipe);
        write(fd_fifo,&number,sizeof(int));
        for (int i = 0; i < number; ++i) {
            num = rand()%100;
            printf("%d\n", num);
            write(fd_fifo,&num,sizeof(int));
            sleep(3);
        }
        sem_post(sem_for_pipe);
    }
    sleep(1);
    sem_wait(sem_for_pipe);
    close(fd_fifo);
    unlink(PIPE_NAME);
    sem_close(sem_for_pipe);
    sem_unlink(SEM_NAME);
    exit(EXIT_SUCCESS);
}
