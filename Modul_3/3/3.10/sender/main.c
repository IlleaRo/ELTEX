#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <stdlib.h>
#include <unistd.h>

#define QUEUE_NAME "/my_queue"
#define PRIORITY 1
#define SIZE 256
#define MAX_MSG 32

int main(){
    mqd_t ds;
    char text[SIZE];
    struct mq_attr queue_attr;
    queue_attr.mq_maxmsg = MAX_MSG;
    queue_attr.mq_msgsize = SIZE;
    queue_attr.mq_flags = 0;
    queue_attr.mq_curmsgs = 0;

    if ((ds = mq_open(QUEUE_NAME,
                      O_CREAT | O_RDWR, 0600)) == (mqd_t)-1){
        perror("Creating queue error");
        exit(EXIT_FAILURE);
    }


    int pFile;
    int bytes_read;
    char buffer[SIZE];
    char* newline_pos;
    if ((pFile = open("text.txt", O_RDONLY)) == -1)
    {
        perror("Can't read file!");
        return 1;
    }
    while ((bytes_read = read(pFile, buffer, SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0'; // Добавляем нулевой символ в конец буфера
        char *ptr = buffer;
        while ((newline_pos = strchr(ptr, '\n')) != NULL) {
            *newline_pos = '\0'; // Заменяем символ новой строки на нулевой символ
            printf("%s\n", ptr); // Выводим строку
            //strcpy(text, ptr);
            if (mq_send(ds, ptr, strlen(ptr)+1, PRIORITY) == -1) {
                perror("Sending message error");
                exit(EXIT_FAILURE);
            }
            ptr = newline_pos + 1; // Перемещаем указатель после символа новой строки
        }
        if (ptr != buffer) {
            if (mq_send(ds, ptr, strlen(ptr)+1, PRIORITY) == -1) {
                perror("Sending message error");
                exit(EXIT_FAILURE);
            }
        }
    }
    if (mq_send(ds, "\0", 1, 255) == -1) {
        perror("Sending message error");
        exit(EXIT_FAILURE);
    }
    if (mq_close(ds) == -1)
        perror("Closing queue error");
    exit(EXIT_SUCCESS);
}