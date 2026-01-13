#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <vector>

#include "crypto.h"
#include "metrics.h"

#define PORT 9000
#define SERVER_IP "127.0.0.1"
#define PAYLOAD_SIZE (64 * 1024)  // 64 KB

int main() {
    int sock_fd;
    sockaddr_in server_addr{};

    // Create socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        return 1;
    }

    // Disable Nagle (low latency)
    int flag = 1;
    setsockopt(sock_fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock_fd);
        return 1;
    }

    // AES setup
    std::vector<unsigned char> key(16, 'K');
    std::vector<unsigned char> iv(16, 'I');

    // Low-copy payload (NO std::string)
    std::vector<unsigned char> plaintext(PAYLOAD_SIZE, 'A');

    // Encrypt
    auto encrypted = aes_encrypt(plaintext, key, iv);

    Timer timer;
    timer.start();

    // Send encrypted payload
    send(sock_fd, encrypted.data(), encrypted.size(), MSG_NOSIGNAL);

    // Receive ACK
    char ack[4] = {0};
    recv(sock_fd, ack, sizeof(ack), 0);

    double latency_ms = timer.stop();
    double mb = encrypted.size() / (1024.0 * 1024.0);
    double throughput = mb / (latency_ms / 1000.0);

    std::cout << "Latency (RTT): " << latency_ms << " ms\n";
    std::cout << "Throughput: " << throughput << " MB/s\n";

    close(sock_fd);
    return 0;
}
