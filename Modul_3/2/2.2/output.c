#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUF_SIZE 50

int main(int argc, char* argv[])
{
    FILE *fp;
    char buf[BUF_SIZE];
    if (argc < 2)
    {
        printf("Empty filename!");
        exit(EXIT_FAILURE);
    }
    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        fprintf(stderr,"Error reading file!\n");
        exit(EXIT_FAILURE);
    }
    fread(buf,BUF_SIZE, 1, fp);
    printf("%s\n", buf);
    fclose(fp);
    exit(EXIT_SUCCESS);
}