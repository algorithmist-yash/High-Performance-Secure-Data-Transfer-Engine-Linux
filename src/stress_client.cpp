#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/tcp.h>

#include "crypto.h"
#include "metrics.h"

#define SERVER_IP "127.0.0.1"
#define PORT 9000
#define PAYLOAD_SIZE (64 * 1024)   // 64 KB
#define CLIENT_COUNT 100           // 🔥 increase to 200 if you want

std::atomic<int> success_count(0);
std::atomic<int> failure_count(0);

void client_task() {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        failure_count++;
        return;
    }

    int flag = 1;
    setsockopt(sock_fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(sock_fd);
        failure_count++;
        return;
    }

    // AES setup
    std::vector<unsigned char> key(16, 'K');
    std::vector<unsigned char> iv(16, 'I');

    std::vector<unsigned char> plaintext(PAYLOAD_SIZE, 'A');
    auto encrypted = aes_encrypt(plaintext, key, iv);

    send(sock_fd, encrypted.data(), encrypted.size(), MSG_NOSIGNAL);

    char ack[4] = {0};
    recv(sock_fd, ack, sizeof(ack), 0);

    success_count++;
    close(sock_fd);
}

int main() {
    std::cout << "Starting stress test with "
              << CLIENT_COUNT << " concurrent clients\n";

    Timer timer;
    timer.start();

    std::vector<std::thread> threads;
    threads.reserve(CLIENT_COUNT);

    for (int i = 0; i < CLIENT_COUNT; ++i) {
        threads.emplace_back(client_task);
    }

    for (auto& t : threads) {
        t.join();
    }

    double elapsed_ms = timer.stop();

    double total_mb =
        (CLIENT_COUNT * PAYLOAD_SIZE) / (1024.0 * 1024.0);
    double throughput = total_mb / (elapsed_ms / 1000.0);

    std::cout << "----------------------------------\n";
    std::cout << "Successful clients: " << success_count << "\n";
    std::cout << "Failed clients    : " << failure_count << "\n";
    std::cout << "Total time        : " << elapsed_ms << " ms\n";
    std::cout << "Aggregate Throughput: "
              << throughput << " MB/s\n";
    std::cout << "----------------------------------\n";

    return 0;
}
