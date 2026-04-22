#include <iostream>
#include <chrono>
#include "heartbeat.hpp"

constexpr int N = 5;

auto last_time = std::chrono::steady_clock::now();
void send_heartbeat() {
    while (true) {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - last_time).count() >= N) {
            // Perform every 5 seconds
            std::cout << "Hello\n";
            last_time = now;
        }
    }
}
