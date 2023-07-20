#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <linux/limits.h>

int main(int argc, char* argv[])
{
    DIR* path;
    if (argc < 2)
    {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) 
        {
            printf("Program will work with current working dir: %s\n", cwd);
        } 
        else 
        {
            perror("getcwd() error");
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
        if ((path = opendir(argv[1])) == NULL)
        {
            perror("Cannot open specified dir!\n");
            exit(EXIT_FAILURE);
        }
    }
    struct dirent* dir;
    int counter = 1;
    while ((dir = readdir(path)) != NULL)
        printf("%d) %s\n", counter++, dir->d_name);
    closedir(path);
    exit(EXIT_SUCCESS);
}