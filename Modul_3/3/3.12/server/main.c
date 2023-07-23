#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SIZE 256
#define QUEUE_NAME_MAIN "/server"
#define MAX_MSG_NUM 10
#define MAX_NUM_USR 10
#define INIT_TYPE 1001
#define TYPE_STOP 1015

char* concat3(char* s1,char sym, char* s2);
char* concat2(char* s1, char* s2);

int main() {
    //mq_unlink("/ALEX");
    //mq_unlink(QUEUE_NAME_MAIN);
    mqd_t ds;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MSG_NUM;
    attr.mq_msgsize = SIZE;
    attr.mq_curmsgs = 0;
    if ((ds = mq_open(QUEUE_NAME_MAIN, O_CREAT | O_RDWR | O_CLOEXEC,
                      0600, &attr)) == -1)
    {
        perror("Creating queue error");
        exit(EXIT_FAILURE);
    }
    mqd_t usrArr[MAX_NUM_USR];
    char* names[MAX_NUM_USR];
    for (int i = 0; i < MAX_NUM_USR; ++i)
        usrArr[i] = -1;
    char text[SIZE];
    unsigned int prio;
    while (1)
    {
        if (mq_receive(ds, text, SIZE, &prio) == -1)
        {
            perror("cannot receive");
            break;
        }
        switch (prio)
        {
            case INIT_TYPE:
            {
                mqd_t tID;
                for (int i = 0; i < MAX_NUM_USR; ++i)
                    if (usrArr[i] == -1)
                    {
                        names[i] = concat2("/",text);
                        if ((tID = mq_open(names[i], O_CREAT | O_RDWR | O_CLOEXEC,
                                          0666, &attr)) == -1)
                        {
                            perror("Creating queue error");
                            exit(EXIT_FAILURE);
                        }
                        usrArr[i] = tID;

                        sprintf(text,"%d",i);
                        mq_send(ds,text, strlen(text)+1, 1);

                        sleep(2);
                        break;
                    }
                break;
            }
            default:
            {
                if (strcmp(text, "exit") == 0)
                {
                    mq_unlink(names[prio]);
                    free(names[prio]);
                    usrArr[prio] = -1;
                    continue;
                }
                    char* tString = concat3(names[prio],':',text);
                    printf("%s\n",tString);
                    for (int i = 0; i < MAX_NUM_USR; ++i)
                        if (usrArr[i] != -1 && i != prio)
                            mq_send(usrArr[i],tString, strlen(tString)+1,1);
                    free(tString);
                break;
            }
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