#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

#define SIZE_OF_INPUT 10

void printSquare(char*,bool);

int main(int argc, char* argv[])
{
    if (argc == 1) exit(EXIT_SUCCESS);
    if (argc == 2)
    {
        printSquare(argv[1],false);
        exit(EXIT_SUCCESS);
    }
    switch (fork())
    {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        for (int i = (argc-1)/2+1; i < argc; i++)
            printSquare(argv[i], true);
        exit(EXIT_SUCCESS);
    default:
        for (int i = 1; i <= (argc-1)/2; i++)
            printSquare(argv[i], false);
    }
    exit(EXIT_SUCCESS);
}

void printSquare(char* str, bool isChild)
{
    static char* endPtr;
    static long num;
    // Текст будет восприниматься как нуль!
    num = strtol(str, &endPtr, SIZE_OF_INPUT);
    if (endPtr==str)
    {
        perror("Input error!");
        exit(EXIT_FAILURE);
    }
    printf("%s: %d^2 = %d\n", isChild? "CHILD":"PARRENT", num, num*num);
}