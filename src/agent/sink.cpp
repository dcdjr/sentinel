#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>

#include "sentinel/agent/sink.hpp"

using json = nlohmann::json;

const char* COLLECTOR_IP = "127.0.0.1";
constexpr int COLLECTOR_PORT = 5150;

static int connect_to_collector() {
    int collector_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (collector_fd < 0) {
        return -1;
    }

    struct sockaddr_in collector_addr;
    collector_addr.sin_family = AF_INET;
    collector_addr.sin_port = htons(COLLECTOR_PORT);
    inet_pton(AF_INET, COLLECTOR_IP, &collector_addr.sin_addr);

    if (connect(collector_fd, (struct sockaddr *)&collector_addr, sizeof(collector_addr)) < 0) {
        // Connection failed
        close(collector_fd);
        return -1;
    }

    return collector_fd;
}

static int send_all(int collector_fd, const char* buffer, int length) {
    int total_sent = 0; // Bytes sent so far
    int bytes_remaining = length; // Bytes left to send
    int n;

    while (total_sent < length) {
        // Offset the buffer pointer by the number of bytes already sent
        n = send(collector_fd, buffer + total_sent, bytes_remaining, 0);

        if (n <= 0) return -1;

        total_sent += n;
        bytes_remaining -= n;
    }

    return total_sent;
}

void sink_stdout(const json& data) {
    std::string data_str = data.dump();
    std::cout << data_str << std::endl;
}

bool sink_tcp(const json& data) {
    std::string data_str = data.dump();
    int collector_fd = connect_to_collector();
    if (collector_fd < 0) {
        std::cerr << "Failed to connect to collector." << std::endl;
        return false;
    }
    
    // Send size first so the receiver knows how much to read
    uint32_t size = htonl(data_str.size()); // convert to network byte order
    int bytesSizeSent = send_all(collector_fd, (const char *)&size, sizeof(size));
    if (bytesSizeSent < 0) {
        std::cerr << "Failed to send data size." << std::endl;
        close(collector_fd);
        return false;
    }

    // send the actual string data
    int bytesSent = send_all(collector_fd, data_str.data(), data_str.size());
    if (bytesSent < 0) {
        std::cerr << "Failed to send data." << std::endl;
        close(collector_fd);
        return false;
    }

    close(collector_fd);

    return true;
}
