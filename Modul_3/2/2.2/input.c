#include <stdio.h>
#include <stdlib.h>


#define PHRASE "Hello from another program!"

int main(int argc, char* argv[])
{
    FILE *fp;
    char buf[] = PHRASE;
    if (argc < 2)
    {
        printf("Empty filename!");
        exit(EXIT_FAILURE);
    }
    if ((fp = fopen(argv[1], "w")) == NULL)
    {
        fprintf(stderr,"Error creating or opening file!\n");
        exit(EXIT_FAILURE);
    }
    fwrite(buf, sizeof(buf)/sizeof(char), 1, fp);
    fclose(fp);
    exit(EXIT_SUCCESS);
}