#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INPUT_SIZE 100

char* concat(char*, char*); // Функция для склеивания строк
int find(const char*,char sym); // Число вхождений символа в строке

int main() {
    char command[INPUT_SIZE];
    int pt;
    char sep[] = " ";
    char sepLine[] = "|";
    printf("=== My command line 0.2 ===\n");
    while (true) {
        fgets(command, INPUT_SIZE, stdin);
        int i, numberOfCommand;
        char prevSym = '\0';
        int counter = 0;
        switch (numberOfCommand = find(command, '|')) {
            case 0: {
                for (i = 0; i < INPUT_SIZE; i++) {
                    if (command[i] == '\n' || command[i] == '\0') {
                        command[i] = '\0';
                        break;
                    }
                    if (command[i] == ' ' && command[i] != prevSym)
                        counter++;
                    prevSym = command[i];
                }
                for (int j = --i; command[j] == ' ' && j > 0; j--)
                    counter--;


                char *argv[++counter + 1];
                argv[0] = concat("/usr/bin/", strtok(command, sep));

                for (int j = 1; j < counter; j++)
                    argv[j] = strtok(NULL, sep);

                argv[counter] = NULL;
                switch (fork()) {
                    case -1:
                        perror("Input error!");
                        exit(EXIT_FAILURE);
                    case 0:
                        execvp(argv[0], argv);
                    default:
                        wait(&pt);
                        break;
                }
                counter = 0;
                free(argv[0]);
                break;
            }
            case 1:
            {
                char* commands[++numberOfCommand];
                commands[0] = strtok(command,sepLine);
                for (int j = 1; j < numberOfCommand; ++j)
                    commands[j] = strtok(NULL,sepLine);

                for (i = 0; i < INPUT_SIZE; i++) {
                    if (commands[0][i] == '\n' || commands[0][i] == '\0') {
                        commands[0][i] = '\0';
                        break;
                    }
                    if (commands[0][i] == ' ' && commands[0][i] != prevSym)
                        counter++;
                    prevSym = commands[0][i];
                }
                for (int j = --i; commands[0][j] == ' ' && j > 0; j--)
                    counter--;

                char *argv[++counter + 1];
                argv[0] = concat("/usr/bin/", strtok(commands[0], sep));

                for (int j = 1; j < counter; j++)
                    argv[j] = strtok(NULL, sep);

                argv[counter] = NULL;
                int pPipe_1[2];
                pipe(pPipe_1);
                switch (fork()) {
                    case -1:
                        perror("Input error!");
                        exit(EXIT_FAILURE);
                    case 0:
                        close(pPipe_1[0]);
                        dup2(pPipe_1[1], STDOUT_FILENO);
                        execvp(argv[0], argv);
                    default:
                        break;
                }
                counter = 0;
                for (i = 1; i < INPUT_SIZE; i++) {
                    if (commands[1][i] == '\n' || commands[1][i] == '\0') {
                        commands[1][i] = '\0';
                        break;
                    }
                    if (commands[1][i] == ' ' && commands[1][i] != prevSym)
                        counter++;
                    prevSym = commands[1][i];
                }
                for (int j = --i; commands[1][j] == ' ' && j > 0; j--)
                    counter--;

                char *argv2[++counter + 1];
                argv2[0] = concat("/usr/bin/", strtok(commands[1], sep));

                for (int j = 1; j < counter; j++)
                    argv2[j] = strtok(NULL, sep);

                argv2[counter] = NULL;

                switch (fork()) {
                    case -1:
                        perror("Input error!");
                        exit(EXIT_FAILURE);
                    case 0:
                        close(pPipe_1[1]);
                        dup2(pPipe_1[0], STDIN_FILENO);
                        execvp(argv2[0], argv2);
                }
                counter = 0;
                free(argv2[0]);
                free(argv[0]);
                break;
            }
            case 2:
            {
                char* commands[++numberOfCommand];
                commands[0] = strtok(command,sepLine);
                for (int j = 1; j < numberOfCommand; ++j)
                    commands[j] = strtok(NULL,sepLine);

                for (i = 0; i < INPUT_SIZE; i++) {
                    if (commands[0][i] == '\n' || commands[0][i] == '\0') {
                        commands[0][i] = '\0';
                        break;
                    }
                    if (commands[0][i] == ' ' && commands[0][i] != prevSym)
                        counter++;
                    prevSym = commands[0][i];
                }
                for (int j = --i; commands[0][j] == ' ' && j > 0; j--)
                    counter--;

                char *argv[++counter + 1];
                argv[0] = concat("/usr/bin/", strtok(commands[0], sep));

                for (int j = 1; j < counter; j++)
                    argv[j] = strtok(NULL, sep);

                argv[counter] = NULL;
                int pPipe_1[2];
                int pPipe_2[2];
                pipe(pPipe_1);
                pipe(pPipe_2);
                switch (fork()) {
                    case -1:
                        perror("Input error!");
                        exit(EXIT_FAILURE);
                    case 0:
                        close(pPipe_1[0]);
                        dup2(pPipe_1[1], STDOUT_FILENO);
                        execvp(argv[0], argv);
                    default:
                        break;
                }
                counter = 0;
                for (i = 1; i < INPUT_SIZE; i++) {
                    if (commands[1][i] == '\n' || commands[1][i] == '\0') {
                        commands[1][i] = '\0';
                        break;
                    }
                    if (commands[1][i] == ' ' && commands[1][i] != prevSym)
                        counter++;
                    prevSym = commands[1][i];
                }
                for (int j = --i; commands[1][j] == ' ' && j > 0; j--)
                    counter--;

                char *argv2[++counter + 1];
                argv2[0] = concat("/usr/bin/", strtok(commands[1], sep));

                for (int j = 1; j < counter; j++)
                    argv2[j] = strtok(NULL, sep);

                argv2[counter] = NULL;

                switch (fork()) {
                    case -1:
                        perror("Input error!");
                        exit(EXIT_FAILURE);
                    case 0:
                        close(pPipe_1[1]);
                        close(pPipe_2[0]);
                        dup2(pPipe_1[0], STDIN_FILENO);
                        dup2(pPipe_2[1],STDOUT_FILENO);
                        execvp(argv2[0], argv2);
                }
                counter = 0;
                if (numberOfCommand < 2)
                {
                    free(argv[0]);
                    free(argv2[0]);
                    continue;
                }

                for (i = 1; i < INPUT_SIZE; i++) {
                    if (commands[2][i] == '\n' || commands[2][i] == '\0') {
                        commands[2][i] = '\0';
                        break;
                    }
                    if (commands[2][i] == ' ' && commands[2][i] != prevSym)
                        counter++;
                    prevSym = commands[2][i];
                }
                for (int j = --i; commands[2][j] == ' ' && j > 0; j--)
                    counter--;

                char *argv3[++counter + 1];
                argv3[0] = concat("/usr/bin/", strtok(commands[2], sep));

                for (int j = 1; j < counter; j++)
                    argv3[j] = strtok(NULL, sep);

                argv3[counter] = NULL;

                switch (fork())
                {
                    case -1:
                        perror("Input error!");
                        exit(EXIT_FAILURE);
                    case 0:
                        close(pPipe_1[0]);
                        close(pPipe_1[1]);
                        close(pPipe_2[1]);
                        dup2(pPipe_2[0],STDIN_FILENO);
                        execvp(argv3[0], argv3);
                }
                close(pPipe_1[0]); // Закрываем дескриптор чтения pipe1
                close(pPipe_1[1]); // Закрываем дескриптор записи pipe1
                close(pPipe_2[0]); // Закрываем дескриптор чтения pipe2
                close(pPipe_2[1]); // Закрываем дескриптор записи pipe2

                counter = 0;
                free(argv2[0]);
                free(argv[0]);
                free(argv3[0]);
                break;
            }
            default:
            {
                printf("Unsupported command!\n");
                break;
            }
        }
    }
}

int find(const char* str, char sym)
{
    int counter = 0, i = 0;
    while (true)
    {
        if (str[i] == '\0' || str[i] == '\n') break;
        if (str[i] == sym) counter++;
        i++;
    }
    return counter;
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