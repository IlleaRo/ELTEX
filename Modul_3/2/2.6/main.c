#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <string.h>

#include "system/input.h"

#define MAX_NUMBER_OF_FILES_IN_PATH 128

char* concat3(char* s1,char sym, char* s2); // Функция для склеивания строк

int main(int argc, char* argv[])
{
    off_t pathArr[MAX_NUMBER_OF_FILES_IN_PATH];
    char* targetDirectory;
    DIR* path;
    char cwd[PATH_MAX];
    if (argc < 2)
    {
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("Program will work with current working dir: %s\n", cwd);
        }
        else
        {
            perror("getcwd() error");
            exit(EXIT_FAILURE);
        }
        if ((chdir(cwd)) == -1)
        {
            perror("Can't change working directory!\n");
            exit(EXIT_FAILURE);
        }
        if ((path = opendir(cwd)) == NULL)
        {
            perror("Cannot open specified dir!\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        if ((chdir(argv[1])) == -1)
        {
            perror("Can't change working directory!\n");
            exit(EXIT_FAILURE);
        }
        if ((path = opendir(argv[1])) == NULL)
        {
            perror("Cannot open specified dir!\n");
            exit(EXIT_FAILURE);
        }
    }
    struct dirent* dir;
    int counter = 0;
    struct stat buf;
    int answer;
    while (1)
    {
        while ((dir = readdir(path)) != NULL)
        {
            printf("%-3d", (counter++)+1);
            stat(dir->d_name,&buf);
            if (S_ISREG(buf.st_mode))
                printf("[FILE]\t");
            else if (S_ISDIR(buf.st_mode))
                printf("[DIR]\t");
            else if (S_ISLNK(buf.st_mode))
                printf("[LINK]\t");
            else
                printf("[UNKNOWN]\t");
            printf("%s\n", dir->d_name);
            pathArr[counter] = telldir(path);
        }
        printf("Choose directory to go ( -1 to exit) ---> ");
        while (1)
        {
            if ((answer = readInt()) < -1 || answer > counter)
            {
                printf("\nIncorrect answer, try again ---> ");
                continue;
            }
            if (answer == -1)
            {
                closedir(path);
                exit(EXIT_SUCCESS);
            }
            seekdir(path,pathArr[answer-1]);
            if ((dir = readdir(path)) != NULL)
            {
                stat(dir->d_name,&buf);
                if (S_ISDIR(buf.st_mode))
                    break;
                else
                    printf("Not a dir!\n ---> ");
            }

        }
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("getcwd!\n");
            exit(EXIT_FAILURE);
        }
        targetDirectory = concat3(cwd,'/',dir->d_name);
        if ((chdir(targetDirectory)) == -1)
        {
            perror("Can't change working directory!\n");
            exit(EXIT_FAILURE);
        }
        if ((path = opendir(targetDirectory)) == NULL)
        {
            perror("Cannot open specified dir!\n");
            exit(EXIT_FAILURE);
        }
        free(targetDirectory);
        counter = 0;
        system("clear");
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