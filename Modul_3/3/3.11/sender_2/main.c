#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <mqueue.h>

#define SIZE 256
#define QUEUE_NAME_SEND "/my_queue_2"
#define QUEUE_NAME_RECEIVE "/my_queue_1"


int main()
{

    pid_t mainPID = getpid();
    pid_t forkPID;
    char text[SIZE];

    switch (forkPID = fork())
    {
        case -1:
            perror("FORK problem!\n");
            exit(EXIT_FAILURE);
        case 0: {
            sleep(2);
            mqd_t ds_receive;
            if ((ds_receive = mq_open(QUEUE_NAME_RECEIVE,
                                      O_RDONLY)) == (mqd_t)-1){
                perror("Creating queue error");
                exit(EXIT_FAILURE);
            }
            unsigned int prio;
            while (1)
            {
                if (mq_receive(ds_receive, text, SIZE, &prio) == -1) {
                    perror("cannot receive");
                    printf("READER IS DEAD!");
                    exit(EXIT_FAILURE);
                }
                if (prio == 255) {
                    printf("Get MSG with priority %d", prio);
                    break;
                }
                printf("Message: %s, prio = %d\n", text, prio);
            }
            printf("READER IS DEAD!");
            exit(EXIT_SUCCESS);
        }
        default:
        {
            mqd_t ds_send;
            //mq_unlink(QUEUE_NAME_SEND);
            struct mq_attr attr;
            attr.mq_flags = 0;      // Флаги (0 или O_NONBLOCK)
            attr.mq_maxmsg = 10;    // Максимальное количество сообщений в очереди
            attr.mq_msgsize = SIZE; // Максимальный размер сообщения в байтах
            attr.mq_curmsgs = 0;    // Текущее количество сообщений в очереди
            if ((ds_send = mq_open(QUEUE_NAME_SEND,
                                   O_CREAT | O_RDWR, 0600, &attr)) == (mqd_t)-1){
                perror("Creating queue error");
                exit(EXIT_FAILURE);
            }

            if (mq_getattr(ds_send, &attr) == -1) {
                perror("mq_getattr");
                mq_close(ds_send);
                return 1;
            }

            // Проверяем максимальный размер сообщения
            if (attr.mq_msgsize > SIZE) {
                printf("Error: Maximum message size exceeds buffer size (%ld).\n", attr.mq_msgsize);
                mq_close(ds_send);
                return 1;
            }

            while (*(fgets(text,SIZE,stdin)) != EOF)
            {
                text[strlen(text)-1] = '\0';
                if (strcmp(text,"exit") == 0)
                {
                    mq_send(ds_send,"\0",1,255);
                    kill(SIGINT,forkPID);
                    mq_close(ds_send);
                    exit(EXIT_SUCCESS);
                }
                mq_send(ds_send,text, strlen(text)+1,1);
            }
        }
    }
}