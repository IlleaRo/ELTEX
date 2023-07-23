#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#define YOUR_MSG_TYPE 2
#define FRIEND_MSG_TYPE 1
#define BUFFER_SIZE 256

typedef struct msgbuf {
    long mtype; /* тип сообщения, должен быть > 0 */
    char mtext[BUFFER_SIZE]; /* содержание сообщения */
} msgbuf;

int main(int argc, char *argv[])
{
   
    int i, msqid;
    key_t keyQ;
    if ((keyQ = ftok(".", 'a')) == -1)
    {
        perror("FTOK error!\n");
        exit(EXIT_FAILURE);
    }
    if ((msqid = msgget(keyQ, IPC_CREAT | 0666)) == -1)
    {
        perror("MSGGET error!\n");  
        exit(EXIT_FAILURE);
    }

    msgbuf msg;
    msg.mtype = 1;
    
    switch (fork())
    {
    case -1:
        perror("FORK problem!\n");
        exit(EXIT_FAILURE);
    case 0:
        while (1)
        {
            if (msgrcv(msqid, &msg, BUFFER_SIZE, FRIEND_MSG_TYPE,
                MSG_NOERROR) == -1 && errno != ENOMSG) 
            {
                perror("msgrcv");
                exit(EXIT_FAILURE);
            }
            // Если тип сообщения равен 255, то завершаем обмен данными
            if (msg.mtype == 255)
            {
                msgsnd(msqid, &msg, sizeof(msg.mtext)+1, 0);
                exit(EXIT_SUCCESS);
            }
            printf("Get msg: %s\n", msg.mtext);
        }
    default:
        msg.mtype = YOUR_MSG_TYPE;
        while (*(fgets(msg.mtext,BUFFER_SIZE,stdin)) != EOF)
        {
            if (strcmp(msg.mtext,"exit\n") == 0)
            {
                msg.mtype = 255;
                msgsnd(msqid, &msg, sizeof(msg.mtext)+1, 0);
                exit(EXIT_SUCCESS);
            }
            msgsnd(msqid,&msg,sizeof(msg.mtext)+1,0);
        }
    }
}