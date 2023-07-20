#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/limits.h>
#include <sys/stat.h>

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
    int counter = 1;
    struct stat buf;
    while ((dir = readdir(path)) != NULL)
    {
        printf("%-3d", counter++);
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
    }
    closedir(path);
    exit(EXIT_SUCCESS);
}