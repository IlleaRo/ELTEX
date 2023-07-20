#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <linux/limits.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>


#define PHRASE "Hello from another program!"

int main(int argc, char* argv[])
{
    int fp;
    char buf[] = PHRASE;
    if (argc < 2)
    {
        printf("Empty filename!");
        exit(EXIT_FAILURE);
    }

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd() error");
        exit(EXIT_FAILURE);
    }
    DIR* path;
    if ((path = opendir(cwd)) == NULL)
    {
        perror("Cannot open specified dir!\n");
        exit(EXIT_FAILURE);
    }
    
    bool isFind = false;

    struct dirent* dir;
    while ((dir = readdir(path)) != NULL)
        if (strcmp(dir->d_name,argv[1]) == 0)
            isFind = true;
    if (!isFind)
    {
        printf("File doesn't exist."
        "Program will create it.\nDo you want to continue? [Y/n] ");
        char answer = getc(stdin);
        if (!(answer == 'Y' || answer == 'y'))
        {
            printf("Abort.\n");
            exit(EXIT_SUCCESS);
        }
    }
    if ((fp = open(argv[1], O_WRONLY | O_CREAT, S_IRWXU)) == -1)
    {
        fprintf(stderr,"Error creating or opening file!\n");
        exit(EXIT_FAILURE);
    }
    write(fp, buf, sizeof(buf)/sizeof(char));
    close(fp);
    exit(EXIT_SUCCESS);
}