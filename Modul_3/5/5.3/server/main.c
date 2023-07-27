#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <limits.h>
#include <arpa/inet.h>
#include <netdb.h>



#define BUF_SIZE 256

void doStuff(int); // функция обслуживания подключившихся пользователей
void error(const char*); // функция обработки ошибок
void printUsers();// печать количества активных пользователей

//Функции обработки данных
int sum(int, int);
int diff(int, int);
int prod (int, int);
int division (int, int);

int nClients = 0; // количество активных пользователей

int main(int argc, char *argv[])
{
    char buff[BUF_SIZE]; // Буфер для различных нужд
    int sockfd, newsockfd; // дескрипторы сокетов
    int portno; // номер порта
    int pid; // id номер потока
    socklen_t clilen; // размер адреса клиента типа socklen_t
    struct sockaddr_in serv_addr, cli_addr; // структура сокета сервера и клиента
    printf("TCP SERVER DEMO\n");
// ошибка в случае если мы не указали порт
    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    // Шаг 1 - создание сокета
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
// Шаг 2 - связывание сокета с локальным адресом
    bzero((char*) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // сервер принимает подключения на все IP-адреса
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
// Шаг 3 - ожидание подключений, размер очереди - 5
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    // Шаг 4 - извлекаем сообщение из очереди (цикл извлечения запросов на подключение)
    while (1)
    {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) error("ERROR on accept");
        nClients++;
// вывод сведений о клиенте
        struct hostent *hst;
        hst = gethostbyaddr((char *)&cli_addr.sin_addr, 4, AF_INET);
        printf("+%s [%s] new connect!\n",
               (hst) ? hst->h_name : "Unknown host",
               (char*)inet_ntoa(cli_addr.sin_addr));
        printUsers();
        pid = fork();
        if (pid < 0) error("ERROR on fork");
        if (pid == 0) {
            close(sockfd);
            doStuff(newsockfd);
            exit(0);
        }
        else close(newsockfd);
    }
    close(sockfd);
    return 0;
}

void doStuff (int sock) {
    ssize_t bytes_recv; // размер принятого сообщения
    int a,b; // переменные для sum
    char buff[BUF_SIZE];
#define str1 "Enter 1 parameter\r\n"
#define str2 "Enter 2 parameter\r\n"
#define str3 "Enter a mode:\n1) sum\n2) diff\n3) prod\n4) division\r\n"
// отправляем клиенту сообщение
    write(sock, str1, sizeof(str1));
// обработка первого параметра
    bytes_recv = read(sock,&buff[0],sizeof(buff));
    if (bytes_recv < 0) error("ERROR reading from socket");
    a = atoi(buff); // преобразование первого параметра в int
// отправляем клиенту сообщение
    write(sock,str2,sizeof(str2));
    bytes_recv = read(sock,&buff[0],sizeof(buff));
    if (bytes_recv < 0) error("ERROR reading from socket");
    b = atoi(buff); // преобразование второго параметра в int
// отправляем клиенту сообщение с просьбой выбрать мод
    write(sock,str3,sizeof(str3));
    bytes_recv = read(sock,buff,sizeof(buff));
    if (bytes_recv < 0) error("ERROR reading from socket");
    char* endPtr;
    long mod =  strtol(buff,&endPtr, 10);
    printf("CHOOSE MODE %ld\n", mod);
    switch (mod) {
        case 1:
            a = sum(a,b);
            break;
        case 2:
            a = diff(a,b);
            break;
        case 3:
            a = prod(a,b);
            break;
        case 4:
            a = division(a,b);
            break;
        default:
            error("ERROR reading from socket");
    }
    printf("RESULT: %d\n", a);
    snprintf(buff, BUF_SIZE, "%d", a); // преобразование результата в строку
    printf("STR: %s",buff);
    buff[strlen(buff)] = '\n'; // добавление к сообщению символа конца строки
// отправляем клиенту результат
    write(sock,&buff[0], sizeof(buff)+1);
    close(sock);
    nClients--; // уменьшаем счетчик активных клиентов
    printf("-disconnect\n");
    printUsers();
}

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void printUsers() {
    if(nClients) {
        printf("%d user on-line\n",
               nClients);
    }
    else {
        printf("No User on line\n");
    }
}

int sum(int a, int b) {
    long long sum = (long) (a) + b;
    if (sum > INT_MAX)
        return INT_MAX;
    if (sum < INT_MIN)
        return INT_MIN;
    return (int) sum;
}

int diff(int a, int b) {
    long long diff = (long) (a) - b;
    if (diff > INT_MAX)
        return INT_MAX;
    if (diff < INT_MIN)
        return INT_MIN;
    return (int) diff;
}

int prod(int a, int b) {
    long long prod = (long) (a) * b;
    if (prod > INT_MAX)
        return INT_MAX;
    if (prod < INT_MIN)
        return INT_MIN;
    return (int) prod;
}

int division(int a, int b) {
    long long division = (long) (a)/b;
    if (division > INT_MAX)
        return INT_MAX;
    if (division < INT_MIN)
        return INT_MIN;
    return (int) division;
}