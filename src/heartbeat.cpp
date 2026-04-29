#include <iostream>
#include <chrono>
#include <nlohmann/json.hpp>
#include "heartbeat.hpp"

using json = nlohmann::json;

constexpr int N = 5;

/* Heartbeat is meant to send system info and other important metadata to collector
   The heartbeat is sent every N seconds, is sent in structured JSON format, and 
   will eventually send over TCP but for now it just sends to stdout. */


auto last_time = std::chrono::system_clock::now();
json heartbeat_event;

void send_heartbeat() {
    while (true) {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        char buf[100];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now_c));
        std::string now_s(buf);
        if (std::chrono::duration_cast<std::chrono::seconds>(now - last_time).count() >= N) {
            // Perform every 5 seconds
            heartbeat_event["timestamp"] = now_s;
            heartbeat_event["CPU"] = "42%";
            heartbeat_event["Memory"] = "10000/16000 bytes";
            std::cout << heartbeat_event;
            last_time = now;
        }
    }
}
