#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8888
#define BUFFER_SIZE 1024

int main()
{
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // 建立 socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 設置伺服器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // 連接伺服器
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server. You can now start typing...\n");

    // 不斷從標準輸入讀取並發送到伺服器
    while (1)
    {
        fgets(buffer, BUFFER_SIZE, stdin);

        // 發送到伺服器
        if (send(client_socket, buffer, strlen(buffer), 0) == -1)
        {
            perror("Sending failed");
            exit(EXIT_FAILURE);
        }
    }

    close(client_socket);
    return 0;
}
