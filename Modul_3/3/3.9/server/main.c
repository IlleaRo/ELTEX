#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#define INIT_TYPE 100
#define BUFFER_SIZE 256
#define MAX_NUMBER_OF_USERS 10
#define MAX_NAME_SIZE 10
#define SPEC_SYM '#'

typedef struct msgbuf {
    long mtype; /* тип сообщения, должен быть > 0 */
    char mtext[BUFFER_SIZE]; /* содержание сообщения */
} msgbuf;

char* concat3(char* s1,char sym, char* s2);

int main() {
    int i, msqid;
    key_t keyQ;
    if ((keyQ = ftok("/home/illearo/Documents/Repositories/ELTEX/Modul_3/3/3.9/.", 'b')) == -1)
    {
        perror("FTOK error!\n");
        exit(EXIT_FAILURE);
    }
    // Удаление очереди
    if ((msqid = msgget(keyQ, IPC_EXCL)) != -1)
    {
        if (msgctl(msqid, IPC_RMID, NULL) == -1)
        {
            fprintf(stderr, "Message queue could not be deleted.\n");
            exit(EXIT_FAILURE);
        }
    }
    if ((msqid = msgget(keyQ, IPC_CREAT | IPC_EXCL | 0666)) == -1)
    {
        perror("MSGGET error!\n");
        exit(EXIT_FAILURE);
    }
    char users[MAX_NUMBER_OF_USERS+1][MAX_NAME_SIZE];
    long channels[MAX_NUMBER_OF_USERS+1];
    for (int j = 0; j < MAX_NUMBER_OF_USERS; ++j) {
        users[j][0] = SPEC_SYM;
        channels[j] = 0;
    }
    msgbuf msg;
    while (1)
    {
        if (msgrcv(msqid, &msg, BUFFER_SIZE, 0,
                   MSG_NOERROR) == -1 && errno != ENOMSG)
        {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
        if (msg.mtype > INIT_TYPE) continue;
        switch (msg.mtype)
        {
            case INIT_TYPE:
                for (int j = 1; j < MAX_NUMBER_OF_USERS+1; ++j)
                {
                    if (users[j][0] == SPEC_SYM)
                    {
                        strcpy(users[j],msg.mtext);
                        channels[j] = time(NULL); // Канал для записи id, канал для чтения rand
                        printf("New USER: %d | %ld\n", j,channels[j]);
                        sprintf(msg.mtext,"%d", j);
                        msgsnd(msqid,&msg, strlen(msg.mtext)+1,0);
                        sprintf(msg.mtext, "%ld", channels[j]);
                        sleep(1);
                        msgsnd(msqid,&msg, strlen(msg.mtext)+1,0);
                        break;
                    }
                }
                break;
            default:
                if (strcmp(msg.mtext,"exit") == 0)
                {
                    users[msg.mtype][0] = SPEC_SYM;
                    channels[msg.mtype] = 0;
                    break;
                }
                char* tString = concat3(users[msg.mtype],':',msg.mtext);
                strcpy(msg.mtext,tString);
                printf("%s\n",msg.mtext);
                long oldMST = msg.mtype;
                for (int j = 1; j < MAX_NUMBER_OF_USERS+1; ++j)
                    if (channels[j] != 0 && j!=oldMST)
                    {
                        msg.mtype = channels[j];
                        msgsnd(msqid,&msg, strlen(msg.mtext)+1,0);
                    }
                free(tString);
        }
    }
}

char* concat3(char* s1,char sym, char* s2)
{
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char* result = (char*) malloc((len1+len2+2)*sizeof(char));
    if (!result)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    memcpy(result,s1,len1);
    result[len1] = sym;
    memcpy(result+len1+1, s2, len2 + 1);
    return result;
}