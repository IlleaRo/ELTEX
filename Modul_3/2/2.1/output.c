#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUF_SIZE 50

int main(int argc, char* argv[])
{
    int fp;
    char buf[BUF_SIZE];
    if (argc < 2)
    {
        printf("Empty filename!");
        exit(EXIT_FAILURE);
    }
    if ((fp = open(argv[1], O_RDONLY)) == -1)
    {
        fprintf(stderr,"Error reading file!\n");
        exit(EXIT_FAILURE);
    }
    read(fp,buf,BUF_SIZE);
    printf("%s\n", buf);
    close(fp);
    exit(EXIT_SUCCESS);
}