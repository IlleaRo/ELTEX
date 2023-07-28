#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 51000 // Порт, на котором слушаем сообщения
#define SERVER_IP "127.0.0.1" // IP-адрес сервера (localhost)

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(struct sockaddr);

    // Создаем UDP сокет
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Заполняем структуру адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // Привязываем сокет к адресу и порту
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка при привязке сокета к адресу и порту");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Буфер для приема сообщения
    char buffer[1024];

    printf("Сервер ожидает сообщения...\n");

    while (1) {
        // Читаем данные из сокета
        ssize_t recv_bytes = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&client_addr, &addr_len);
        if (recv_bytes < 0) {
            perror("Ошибка при чтении данных из сокета");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < recv_bytes; ++i) {
            printf("%02x ", buffer[i]);
            if (i % 16 == 0 && i > 0)
                printf("\n");
        }
    }

    // Закрываем сокет
    close(sockfd);

    return 0;
}