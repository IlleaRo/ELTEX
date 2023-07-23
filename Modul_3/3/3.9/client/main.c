#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define INIT_TYPE 100
#define INPUT_SIZE 10
#define BUFFER_SIZE 256

typedef struct msgbuf {
    long mtype; /* тип сообщения, должен быть > 0 */
    char mtext[BUFFER_SIZE]; /* содержание сообщения */
} msgbuf;

int main(int argc, char* argv[]) {
    if (argc == 1)
    {
        printf("You sign in us unknown");
    }
    key_t keyQ;
    int msqid;
    msgbuf msg;

    if ((keyQ = ftok("/home/illearo/Documents/Repositories/ELTEX/Modul_3/3/3.9/.", 'b')) == -1)
    {
        perror("FTOK error!\n");
        exit(EXIT_FAILURE);
    }
    if ((msqid = msgget(keyQ, 0666)) == -1)
    {
        perror("MSGGET error!\n");
        exit(EXIT_FAILURE);
    }
    // Посылаем запрос серверу
    msg.mtype = INIT_TYPE;
    if (argc == 1)
        strcpy(msg.mtext,"unknown");
    else
        strcpy(msg.mtext,argv[1]);
    msgsnd(msqid,&msg, strlen(msg.mtext)+1,0);
    // Получаем ID для записи
    if (msgrcv(msqid, &msg, BUFFER_SIZE, 100,
               MSG_NOERROR) == -1)
    {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    char input[INPUT_SIZE];
    char* endPtr;
    long id_read, id_write;
    if ((id_write = strtol(msg.mtext,&endPtr,10)) == 0)
    {
        perror("Can't get ID\n");
        exit(EXIT_FAILURE);
    }
    printf("YOU ID TO WRITE - %ld\n",id_write);
    // Получаем ID для чтения
    if (msgrcv(msqid, &msg, BUFFER_SIZE, 100,
               MSG_NOERROR) == -1)
    {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    if ((id_read = strtol(msg.mtext,&endPtr,10)) == 0)
    {
        perror("Can't get ID\n");
        exit(EXIT_FAILURE);
    }
    printf("YOU ID TO READ - %ld\n",id_read);
    // Считываем и выводим сообщения из очереди
    int pFork;
    switch ((pFork = fork()))
    {
        case -1:
            perror("FORK!\n");
            exit(EXIT_FAILURE);
        case 0:
            while (1)
            {
                if (msgrcv(msqid, &msg, BUFFER_SIZE, id_read,
                           MSG_NOERROR) == -1)
                {
                    perror("msgrcv");
                    exit(EXIT_FAILURE);
                }
                printf("%s\n",msg.mtext);
            }
        default:
            msg.mtype = id_write;
            while (*(fgets(msg.mtext,BUFFER_SIZE,stdin)) != EOF)
            {
                msg.mtext[strlen(msg.mtext)-1] = '\0';
                if (strcmp(msg.mtext,"exit") == 0)
                {
                    msgsnd(msqid, &msg, sizeof(msg.mtext)+1, 0);
                    kill(SIGINT,pFork);
                    exit(EXIT_SUCCESS);
                }
                msgsnd(msqid,&msg,sizeof(msg.mtext)+1,0);
            }
    }
    return 0;
}