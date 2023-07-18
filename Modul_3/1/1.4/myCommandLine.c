#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define INPUT_SIZE 100

int main()
{
    char command[100];
    char sep[] = " ";
    int pt;
    printf("=== My command line 0.1 ===");
    while(true)
    {
        printf("\n---> ");
        fgets(command,INPUT_SIZE,stdin);
        int i; char prevSym = '\0'; int counter = 0;
        for (i = 0; i < INPUT_SIZE; i++)
        {
            if (command[i] == '\n' || command[i] == '\0') break;
            if (command[i] == ' ' && command[i] != prevSym)
                counter++;
            prevSym = command[i];
        }
        for (int j = --i; command[j] == ' ' && j > 0; j--)
            counter--;
        char* argv[++counter+1];
        argv[0] = strtok(command,sep);
        for (int j = 1; j < counter; j++)
            argv[j] = strtok(NULL, sep);
        argv[counter] = NULL;
        switch (fork())
        {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            execv(argv[0],argv);
        default:
            wait(&pt);
            break;
        }
	counter = 0;
    }
    exit(EXIT_SUCCESS);
}
