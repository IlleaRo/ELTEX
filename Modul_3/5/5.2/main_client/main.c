#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define MSG_LEN 256

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: a.out <Port>\n");
        exit(EXIT_FAILURE);
    }
    int sockfd; /* Дескриптор сокета */
    socklen_t clilen; /* Переменные для различных длин и количества символов */
    ssize_t n;
    struct sockaddr_in servaddr, cliaddr; /* Структуры для адресов сервера и клиента */
/* Заполняем структуру для адреса сервера */
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;

    char* endPtr;
    long t_port = strtol(argv[1],&endPtr,10);
    if (t_port == 0)
    {
        printf("Invalid port\n");
        close(sockfd); /* По окончании работы закрываем дескриптор сокета */
        exit(EXIT_FAILURE);
    }
    servaddr.sin_port = htons(t_port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
/* Создаем UDP сокет */
    if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
        perror(NULL); /* Печатаем сообщение об ошибке */
        exit(EXIT_FAILURE);
    }
    /* Настраиваем адрес сокета */
    if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        perror(NULL);
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    pid_t pFork;
    clilen = sizeof(cliaddr);

    char line[MSG_LEN];
    if((n = recvfrom(sockfd, line, MSG_LEN, 0,
                     (struct sockaddr *) &cliaddr, &clilen)) < 0){
        perror(NULL);
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("%s", line);

    switch (pFork = fork()) {
        case -1:
            perror("FORK");
            exit(EXIT_FAILURE);
        case 0:
            while (1){
                if((n = recvfrom(sockfd, line, MSG_LEN, 0,
                                 (struct sockaddr *) &cliaddr, &clilen)) < 0){
                    perror(NULL);
                    close(sockfd);
                    exit(EXIT_FAILURE);
                }
                printf("%s", line);
            }
    }
    while(1) {
        fgets(line, MSG_LEN, stdin);
/* Принятый текст отправляем обратно по адресу отправителя */
        if (strcmp(line, "exit\n") == 0) {
            close(sockfd);
            kill(pFork, SIGINT);
            exit(EXIT_SUCCESS);
        }
        if (sendto(sockfd, line, strlen(line) + 1, 0,
                   (struct sockaddr *) &cliaddr, clilen) < 0) {
            perror(NULL);
            close(sockfd);
            exit(EXIT_FAILURE);
        } /* Уходим ожидать новую датаграмму*/
    }
    exit(EXIT_SUCCESS);
}