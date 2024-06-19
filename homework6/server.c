#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8888
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// 全局變數，用於維護客戶端數量和連接描述符
int client_count = 0;
int client_sockets[MAX_CLIENTS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// 函數原型
void *handle_client(void *arg);

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pthread_t tid;

    // 建立 socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 設置伺服器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // 綁定伺服器地址
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // 監聽連接
    if (listen(server_socket, 5) == -1)
    {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    printf("Server started. Listening on port %d...\n", PORT);

    // 接受客戶端連接並創建獨立線程處理每個客戶端
    while (1)
    {
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len)) == -1)
        {
            perror("Acceptance failed");
            continue;
        }

        // 加鎖，更新客戶端連接資訊
        pthread_mutex_lock(&mutex);
        client_sockets[client_count++] = client_socket;
        pthread_mutex_unlock(&mutex);

        // 創建線程處理客戶端
        pthread_create(&tid, NULL, handle_client, &client_socket);
    }

    close(server_socket);
    return 0;
}

void *handle_client(void *arg)
{
    int client_socket = *(int *)arg;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    // 接收和轉發客戶端的輸入
    while ((bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0)
    {
        buffer[bytes_read] = '\0'; // 添加字符串結束標誌
        printf("Client %d: %s", client_socket, buffer);

        // 廣播給所有客戶端
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < client_count; ++i)
        {
            if (client_sockets[i] != client_socket)
            {
                send(client_sockets[i], buffer, strlen(buffer), 0);
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    // 如果接收錯誤或客戶端斷開連接，清除該客戶端
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < client_count; ++i)
    {
        if (client_sockets[i] == client_socket)
        {
            while (i < client_count - 1)
            {
                client_sockets[i] = client_sockets[i + 1];
                i++;
            }
            break;
        }
    }
    client_count--;
    pthread_mutex_unlock(&mutex);

    close(client_socket);
    pthread_exit(NULL);
}
