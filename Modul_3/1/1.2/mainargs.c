#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


void printArgs(int, char*[]);
void finishPrint(int, void*);

int main(int argc, char* argv[])
{
    
    int rv;
    pid_t pid = fork();
    pid_t ownPid = getpid();
    on_exit(finishPrint, &ownPid);
    switch (pid)
    {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        printf("CHILD: PID - %d\nCHILD: PPID - %d\n",
            getpid(),getppid());
        printf("CHILD ARGS: ");
        printArgs(argc,argv);
        exit(EXIT_SUCCESS);
    default:
        printf("PARENT: PID - %d\nPARENT: CHILD PID - %d\n",
            getpid(),pid);
        printf("PARRENT ARGS: ");
        printArgs(argc,argv);
        wait(&rv);
        printf("PARENT: RETURN STATUS FOR CHILD - %d\n", WEXITSTATUS(rv));
        break;
    }
    exit(EXIT_SUCCESS);
    
}

void printArgs(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++)
        printf("%s ", argv[i]);
    printf("\n");
}

void finishPrint(int argc, void* pid)
{
    printf("Proccess with PID - %d was finished!\n", *(pid_t*)pid);
}