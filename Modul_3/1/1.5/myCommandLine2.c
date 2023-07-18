#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define INPUT_SIZE 100

char* concat(char*, char*); // Функция для склеивания строк

int main()
{
    char command[100];
    char sep[] = " ";
    int pt;
    printf("=== My command line 0.2 ===");
    while(true)
    {
        printf("\n---> ");
        fgets(command,INPUT_SIZE,stdin);
        int i; char prevSym = '\0'; int counter = 0;
        for (i = 0; i < INPUT_SIZE; i++)
        {
            if (command[i] == '\n' || command[i] == '\0')
            {
                command[i] = '\0';
                break;
            }
            if (command[i] == ' ' && command[i] != prevSym)
                counter++;
            prevSym = command[i];
        }
        for (int j = --i; command[j] == ' ' && j > 0; j--)
            counter--;
        char* argv[++counter+1];
        argv[0] = concat("/usr/bin/",strtok(command,sep));
        for (int j = 1; j < counter; j++)
            argv[j] = strtok(NULL, sep);
        argv[counter] = NULL;
        switch (fork())
        {
        case -1:
            perror("Input error!");
            exit(EXIT_FAILURE);
        case 0:
            execv(argv[0],argv);
        default:
            wait(&pt);
            break;
        }
	counter = 0;
    free(argv[0]);
    }
    exit(EXIT_SUCCESS);
}

char* concat(char* s1, char* s2)
{
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char* result = (char*) malloc((len1+len2+1)*sizeof(char));
    if (!result)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    memcpy(result,s1,len1);
    memcpy(result+len1, s2, len2 + 1);
    return result;
}