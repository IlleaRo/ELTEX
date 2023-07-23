#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_TYPE 1 // Тип сообщения для обмена данными
#define BUFFER_SIZE 256

typedef struct msgbuf {
    long mtype; /* тип сообщения, должен быть > 0 */
    char mtext[BUFFER_SIZE]; /* содержание сообщения */
} msgbuf;

int main() {
    key_t keyQ;
    int msqid;
    msgbuf msg;

    if ((keyQ = ftok(".", 'a')) == -1)
    {
        perror("FTOK error!\n");
        exit(EXIT_FAILURE);
    }
    if ((msqid = msgget(keyQ, 0666)) == -1)
    {
        perror("MSGGET error!\n");  
        exit(EXIT_FAILURE);
    }
    // Считываем и выводим сообщения из очереди
    while (1) 
    {
        //msgrcv(msqid, &msg, BUFFER_SIZE+1, MSG_TYPE, 0);


        if (msgrcv(msqid, &msg, BUFFER_SIZE, 0,
            MSG_NOERROR | IPC_NOWAIT) == -1) 
        {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }


        // Если тип сообщения равен 255, то завершаем обмен данными
        if (msg.mtype == 255) {
            break;
        }
        printf("Get msg: %s\n", msg.mtext);
    }

    return 0;
}