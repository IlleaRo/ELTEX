#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
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
    if ((fp = open(argv[1], O_WRONLY | O_CREAT, S_IRWXU)) == -1)
    {
        fprintf(stderr,"Error creating or opening file!\n");
        exit(EXIT_FAILURE);
    }
    write(fp, buf, sizeof(buf)/sizeof(char));
    close(fp);
    exit(EXIT_SUCCESS);
}