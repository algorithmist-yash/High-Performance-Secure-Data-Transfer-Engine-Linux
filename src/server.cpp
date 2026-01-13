#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netinet/tcp.h>
#include <vector>

#include "crypto.h"

#define PORT 9000
#define BUFFER_SIZE 65536

struct client_args {
    int client_fd;
};

void* handle_client(void* arg) {
    client_args* args = (client_args*)arg;
    int client_fd = args->client_fd;
    delete args;

    // Disable Nagle
    int flag = 1;
    setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));

    // Low-copy receive buffer
    std::vector<unsigned char> ciphertext(BUFFER_SIZE);
    ssize_t bytes = recv(client_fd, ciphertext.data(), BUFFER_SIZE, 0);
    if (bytes <= 0) {
        close(client_fd);
        pthread_exit(nullptr);
    }
    ciphertext.resize(bytes);

    // AES setup
    std::vector<unsigned char> key(16, 'K');
    std::vector<unsigned char> iv(16, 'I');

    auto decrypted = aes_decrypt(ciphertext, key, iv);

    std::cout << "[Thread " << pthread_self()
              << "] Received " << decrypted.size()
              << " bytes (first 16 bytes: "
              << std::string(decrypted.begin(), decrypted.begin() + 16)
              << " ...)\n";

    // Send ACK
    const char* ack = "ACK";
    send(client_fd, ack, 3, MSG_NOSIGNAL);

    close(client_fd);
    pthread_exit(nullptr);
}

int main() {
    int server_fd;
    sockaddr_in server_addr{}, client_addr{};
    socklen_t client_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 16) < 0) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    std::cout << "[Server] Optimized multi-threaded server running on port "
              << PORT << "...\n";

    while (true) {
        int client_fd = accept(server_fd,
                               (struct sockaddr*)&client_addr,
                               &client_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        client_args* args = new client_args;
        args->client_fd = client_fd;

        pthread_t tid;
        pthread_create(&tid, nullptr, handle_client, args);
        pthread_detach(tid);
    }

    close(server_fd);
    return 0;
}
