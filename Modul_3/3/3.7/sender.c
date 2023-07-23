#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/msg.h>
#include <fcntl.h>

#define MSG_TYPE 1
#define BUFFER_SIZE 256

typedef struct msgbuf {
    long mtype; /* тип сообщения, должен быть > 0 */
    char mtext[BUFFER_SIZE]; /* содержание сообщения */
} msgbuf;

int main(int argc, char *argv[])
{
   
    int i, msqid;
    key_t keyQ;
    if ((keyQ = ftok(".", 'a')) == -1)
    {
        perror("FTOK error!\n");
        exit(EXIT_FAILURE);
    }


    if ((msqid = msgget(keyQ, IPC_EXCL)) != -1)
    {
        // Удаление очереди
        if (msgctl(msqid, IPC_RMID, NULL) == -1) 
        {
            fprintf(stderr, "Message queue could not be deleted.\n");
            exit(EXIT_FAILURE);
        }
    }

    if ((msqid = msgget(keyQ, IPC_CREAT | IPC_EXCL | 0666)) == -1)
    {
        perror("MSGGET error!\n");  
        exit(EXIT_FAILURE);
    }
    int pFile, bytes_read;
    char buffer[256];
    char* newline_pos; // Указатель на символ новой строки

    if ((pFile = open("text.txt", O_RDONLY)) == -1) 
    {
        perror("Can't read file!");
        return 1;
    }
    msgbuf msg;
    msg.mtype = 1;
    while ((bytes_read = read(pFile, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0'; // Добавляем нулевой символ в конец буфера
        char* ptr = buffer;
        while ((newline_pos = strchr(ptr, '\n')) != NULL) {
            *newline_pos = '\0'; // Заменяем символ новой строки на нулевой символ
            printf("%s\n", ptr); // Выводим строку
            strcpy(msg.mtext,ptr);
            msgsnd(msqid,&msg,sizeof(msg.mtext),0);
            ptr = newline_pos + 1; // Перемещаем указатель после символа новой строки
        }

        // Если в последнем блоке данных не найден символ новой строки, то сохраняем его для следующей итерации
        if (msg.mtext != buffer) {
            strcpy(msg.mtext,ptr);
            msgsnd(msqid, &msg, sizeof(msg.mtext)+1, 0);
        }
    }
    msg.mtype = 255;
    msgsnd(msqid, &msg, sizeof(msg.mtext)+1, 0);

}