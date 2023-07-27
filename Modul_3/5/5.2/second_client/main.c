#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define MSG_LEN 256

int main(int argc, char **argv) {
    int sockfd; /* Дескриптор сокета */
    ssize_t n, len; /* Переменные для различных длин и количества символов */
    char sendline[1000], recvline[1000]; /* Массивы для отсылаемой и принятой строки */
    struct sockaddr_in servaddr, cliaddr; /* Структуры для адресов сервера и клиента */
/* Сначала проверяем наличие второго аргумента в
командной строке. При его отсутствии ругаемся и прекращаем работу */
    if (argc != 2) {
        printf("Usage: a.out <IP address>:<Port>\n");
        exit(EXIT_FAILURE);
    }
    /* Создаем UDP сокет */
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror(NULL); /* Печатаем сообщение об ошибке */
        exit(EXIT_FAILURE);
    }
/* Заполняем структуру для адреса клиента */
    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(0);
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
/* Настраиваем адрес сокета */
    if (bind(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0) {
        perror(NULL);
        close(sockfd); /* По окончании работы закрываем дескриптор сокета */
        exit(EXIT_FAILURE);
    }
/* Заполняем структуру для адреса сервера */
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    if (inet_aton(strtok(argv[1],":"), &servaddr.sin_addr) == 0) {
        printf("Invalid IP address\n");
        close(sockfd); /* По окончании работы закрываем дескриптор сокета */
        exit(EXIT_FAILURE);
    }
    char* endPtr;
    char* str_port = strtok(NULL,":");
    if (str_port == NULL){
        printf("Invalid PORT\n");
        close(sockfd); /* По окончании работы закрываем дескриптор сокета */
        exit(EXIT_FAILURE);
    }
    long t_port = strtol(str_port,&endPtr,10);
    if (t_port == 0)
    {
        printf("Invalid PORT\n");
        close(sockfd); /* По окончании работы закрываем дескриптор сокета */
        exit(EXIT_FAILURE);
    }
    servaddr.sin_port = htons(t_port);
    /* Вводим строку, которую отошлем серверу */


    pid_t pFork;
    switch (pFork = fork()) {
        case -1:
            perror("FORK");
            exit(EXIT_FAILURE);
        case 0:
            char recLine[MSG_LEN];
            while (1){
                if((n = recvfrom(sockfd, recLine, MSG_LEN, 0,
                                 (struct sockaddr *) NULL, NULL)) < 0){
                    perror(NULL);
                    close(sockfd);
                    exit(EXIT_FAILURE);
                }
                printf("%s\n", recLine);
            }
    }
    char sendLine[MSG_LEN];
    while(1) {
        fgets(sendLine, MSG_LEN, stdin);
/* Принятый текст отправляем обратно по адресу отправителя */
        if (strcmp(sendLine, "exit\n") == 0) {
            close(sockfd);
            kill(pFork, SIGINT);
            exit(EXIT_SUCCESS);
        }
        if (sendto(sockfd, sendLine, strlen(sendLine) + 1, 0,
                   (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
            perror(NULL);
            close(sockfd);
            exit(EXIT_FAILURE);
        } /* Уходим ожидать новую датаграмму*/
    }
    exit(EXIT_SUCCESS);
}