#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define SIZE 256
#define QUEUE_NAME_MAIN "/server"
#define MAX_MSG_NUM 10
#define MAX_NUM_USR 10
#define INIT_TYPE 1001
#define TYPE_STOP 1015

char* concat2(char* s1, char* s2);

int main(int argc, char* argv[]) {
    mqd_t ds;
    char* endPtr;
    char text[SIZE];
    if ((ds = mq_open(QUEUE_NAME_MAIN, O_RDWR)) == -1)
    {
        perror("Creating queue error");
        exit(EXIT_FAILURE);
    }
    char* nname;
    if (argc < 2)
    {

        nname = concat2("/","Unknown");
        printf("You sign in us unknown");
        mq_send(ds,"Unknown",8,INIT_TYPE);
    }
    else
    {
        nname = concat2("/",argv[1]);
        mq_send(ds,argv[1],strlen(argv[1])+1,INIT_TYPE);
    }
    unsigned int prio;
    mqd_t ds_read;
    unsigned int ID;
    if ((mq_receive(ds,text,
                    SIZE,&prio)) == -1)
    {
        perror("cannot receive");
        mq_close(ds);
        exit(EXIT_FAILURE);
    }
    ID = (int) strtol(text,&endPtr,10);

    pid_t pid;
    switch (pid = fork())
    {
        case -1:
            perror("FORK!\n");
            exit(EXIT_FAILURE);
        case 0:
        {
            while (1)
            {
                if ((ds_read = mq_open(nname,O_RDONLY)) == -1)
                {
                    perror("Creating queue error");
                    exit(EXIT_FAILURE);
                }
                sleep(3);
                if (mq_receive(ds_read,text,SIZE,&prio) == -1)
                {
                    perror("msgrcv");
                    exit(EXIT_FAILURE);
                }
                printf("%s\n",text);
            }
        }

        default:
            while (*(fgets(text,SIZE,stdin)) != EOF)
            {
                text[strlen(text)-1] = '\0';
                if (strcmp(text,"exit") == 0)
                {
                    mq_send(ds,"\0",1,ID);
                    kill(SIGINT,pid);
                    mq_close(ds);
                    exit(EXIT_SUCCESS);
                }
                mq_send(ds,text, strlen(text)+1,ID);
            }
    }

    exit(EXIT_SUCCESS);
}


char* concat2(char* s1, char* s2)
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
    memcpy(result+len1, s2, len2 + 1);
    return result;
}