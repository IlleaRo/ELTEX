#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/limits.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#define MAX_NUM_FILE 20
#define BUF_SIZE 256

void doStuff(int); // функция обслуживания подключившихся пользователей
void error(const char*); // функция обработки ошибок
void printUsers();// печать количества активных пользователей

int nClients = 0; // количество активных пользователей
int fCounter = 0; // количество файлов
struct dirent* files[MAX_NUM_FILE];

int main(int argc, char* argv[])
{
    // Перемещение в папку и получение ее содержимого
    DIR* path;
    if (argc < 3)
    {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("Program will work with current working dir: %s\n", cwd);
        }
        else
        {
            perror("getcwd() error");
            exit(EXIT_FAILURE);
        }
        if ((path = opendir(cwd)) == NULL)
        {
            perror("Cannot open specified dir!\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        if ((path = opendir(argv[2])) == NULL)
        {
            perror("Cannot open specified dir!\n");
            exit(EXIT_FAILURE);
        }
    }
    for (fCounter = 0; fCounter < MAX_NUM_FILE; ++fCounter) {
        if ((files[fCounter] = readdir(path)) == NULL) break;
        if (files[fCounter]->d_name[0] == '.'){
            fCounter--;
            continue;
        }
        printf("%d) %s\n", fCounter + 1, files[fCounter]->d_name);
    }
    int sockfd, newsockfd; // дескрипторы сокетов
    int portno; // номер порта
    int pid; // id номер потока
    socklen_t clilen; // размер адреса клиента типа socklen_t
    struct sockaddr_in serv_addr, cli_addr; // структура сокета сервера
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
    closedir(path);
    exit(EXIT_SUCCESS);
}

void doStuff (int sock) {
    ssize_t bytes_recv; // размер принятого сообщения
    int a;
    char buff[BUF_SIZE];
    snprintf(buff,BUF_SIZE,"%d",fCounter);
    // отправляем клиенту сообщение с количеством имеющихся файлов
    write(sock, buff, strlen(buff)+1);
    // отправляем пользователю список имеющихся файлов
    for (int i = 0; i < fCounter; ++i)
        write(sock,files[i]->d_name, strlen(files[i]->d_name)+1);
    // получаем от пользователя номер файла, который необходимо передать
    bytes_recv = read(sock,&buff[0],BUF_SIZE);
    if (bytes_recv < 0) error("ERROR reading from socket");
    printf("Пришло %s\n", buff);
    a = atoi(buff); // преобразование первого параметра в int
    a--;
    FILE* pFile;
    printf("Выбран %s",files[a]->d_name);
    // Открываем необходимый файл
    if ((pFile = fopen(files[a]->d_name,"rb")) == NULL){
        error("FOPEN");
        shutdown(sock,SHUT_RDWR);
        close(sock);
        exit(EXIT_FAILURE);
    }
    // Отправляем имя файла
    write(sock,files[a]->d_name, strlen(files[a]->d_name)+1);
    unsigned long bytes_read;
    // Читаем файл и отправляем
    while ((bytes_read = fread(buff,sizeof(char),BUF_SIZE-1,pFile)) > 0)
        write(sock,buff, bytes_read);
    // Завершаем работу
    shutdown(sock,SHUT_RDWR);
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