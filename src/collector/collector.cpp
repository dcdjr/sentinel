#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#include "sentinel/collector/collector.hpp"

constexpr int LISTENING_PORT = 5150;
constexpr int PAYLOAD_SIZE_BYTES = 4;
constexpr int MAX_EVENT_BYTES = 1024;

static int set_up_collector_socket() {
    int collector_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (collector_fd < 0) {
        std::cerr << "Collector socket failed to initialize." << std::endl;
        return -1;
    }

    // Enable SO_REUSEADDR so collector OS allows the port to be reused quickly    
    int opt = 1;
    setsockopt(collector_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(LISTENING_PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int bind_result = bind(collector_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (bind_result < 0) {
        std::cerr << "Collector socket failed to bind." << std::endl;
        close(collector_fd);
        return -1;
    }
    
    int backlog = 10;
    int listen_result = listen(collector_fd, backlog);
    if (listen_result < 0) {
        std::cerr << "Collector failed to listen on socket." << std::endl;
        close(collector_fd);
        return -1;
    }

    return collector_fd;
}

static bool recv_all(int agent_fd, void* buffer, size_t length) {
    char* ptr = static_cast<char*>(buffer);
    while (length > 0) {
        ssize_t received = recv(agent_fd, ptr, length, 0);

        if (received < 0) {
            return false;
        } else if (received == 0) {
            return false;
        }

        ptr += received; // Move buffer pointer forward
        length -= received; // Decrease remaining bytes to read
    }

    return true;
}

void collect() {
    int collector_fd = set_up_collector_socket();
    if (collector_fd < 0) {
        std::cerr << "Collector socket failed to initialize." << std::endl;
        return;
    }

    struct sockaddr_in agent_addr;
    socklen_t agent_len = sizeof(agent_addr);

    while (true) {
        int agent_fd = accept(collector_fd, (struct sockaddr *)&agent_addr, &agent_len);
        if (agent_fd < 0) return;
        
        // Receive 4 bytes for size
        uint32_t size = 0;
        bool received_size = recv_all(agent_fd, &size, PAYLOAD_SIZE_BYTES);
        size = ntohl(size);
    
        if (size > MAX_EVENT_BYTES) {
            std::cerr << "Size of event exceed 1KB." << std::endl;
            continue;
        }

        if (!received_size) {
            std::cerr << "Failed to receive size of payload." << std::endl;
            continue;
        }

        // Receive rest of message
        char* buffer = new char[size + 1];
        bool received_data = recv_all(agent_fd, buffer, size);
        buffer[size] = '\0';
        if (!received_data) {
            std::cerr << "Failed to receive payload." << std::endl;
            continue;
        }

        std::string message(buffer);
        delete[] buffer;
        
        std::cout << message << std::endl;

        close(agent_fd);
    }
}
