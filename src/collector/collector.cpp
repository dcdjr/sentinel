#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <nlohmann/json.hpp>

#include "sentinel/collector/collector.hpp"

using json = nlohmann::json;

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

static bool check_json_fields(const json& j) {
    return (
        j.contains("event_type") &&
        j["event_type"].is_string() &&
        j.contains("timestamp") &&
        j["timestamp"].is_string() &&
        j.contains("agent_id") &&
        j["agent_id"].is_number_integer() &&
        j.contains("hostname") &&
        j["hostname"].is_string() &&
        j.contains("sequence") &&
        j["sequence"].is_number_integer() &&
        j.contains("payload") &&
        j["payload"].is_object()
    );
}

void collect() {
    int collector_fd = set_up_collector_socket();
    if (collector_fd < 0) {
        std::cerr << "Collector socket failed to initialize." << std::endl;
        return;
    }

    struct sockaddr_in agent_addr;

    while (true) {
        socklen_t agent_len = sizeof(agent_addr);

        int agent_fd = accept(collector_fd, (struct sockaddr *)&agent_addr, &agent_len);
        if (agent_fd < 0) return;
        
        // Receive 4 bytes for size
        uint32_t size = 0;
        bool received_size = recv_all(agent_fd, &size, PAYLOAD_SIZE_BYTES);

        if (!received_size) {
            std::cerr << "Failed to receive size of payload." << std::endl;
            close(agent_fd);
            continue;
        }

        size = ntohl(size);

        if (size > MAX_EVENT_BYTES) {
            std::cerr << "Size of event exceeds 1KB." << std::endl;
            close(agent_fd);
            continue;
        } else if (size == 0) {
            std::cerr << "Size of event is invalid." << std::endl;
            close(agent_fd);
            continue;
        }

        // Receive rest of message
        std::string message(size, '\0');
        bool received_data = recv_all(agent_fd, message.data(), size);
        if (!received_data) {
            std::cerr << "Failed to receive payload." << std::endl;
            close(agent_fd);
            continue;
        }

        // Validate that JSON message parses
        try {
            auto j = json::parse(message);

            // If it parses, check that it contains correct fields
            if (!check_json_fields(j)) {
                std::cerr << "Invalid JSON received." << std::endl;
                close(agent_fd);
                continue;
            }

            // Log events

            std::string event_type = j["event_type"];
            int agent_id = j["agent_id"];
            std::string hostname = j["hostname"];
            int sequence = j["sequence"];

            std::cout 
                << "Accepted Event: type=" << event_type 
                << " agent_id=" << agent_id 
                << " hostname=" << hostname
                << " sequence=" << sequence
                << std::endl;

            close(agent_fd);
        } catch (const json::parse_error& e) {
            std::cerr << "Invalid JSON received." << std::endl;
            close(agent_fd);
            continue;
        }
    }
}
