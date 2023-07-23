/* Прием сообщения из очереди */
#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>

#define QUEUE_NAME "/my_queue"
#define PRIORITY 1
#define SIZE 256

#define MAX_MSG 32

int main(){
    mqd_t ds;
    if ((ds = mq_open(QUEUE_NAME,
                      O_RDONLY)) == (mqd_t)-1){
        perror("Creating queue error");
        exit(EXIT_FAILURE);
    }


    char new_text[SIZE];
    unsigned int prio;

    while (1)
    {
        if (mq_receive(ds, new_text, SIZE, &prio) == -1){
            perror("cannot receive");
            return -1;
        }
        if (prio == 255)
        {
            printf("Get MSG with priority %d", prio);
            break;
        }
        printf("Message: %s, prio = %d\n", new_text, prio);
    }

    if (mq_close(ds) == -1)
        perror("Closing queue error");
    if (mq_unlink(QUEUE_NAME) == -1)
        perror("Removing queue error");
    exit(EXIT_SUCCESS);
}