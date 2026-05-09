#include <chrono>
#include <nlohmann/json.hpp>
#include <thread>

#include "sentinel/agent/heartbeat.hpp"
#include "sentinel/agent/sink.hpp"
#include "sentinel/common/event.hpp"

using json = nlohmann::json;

constexpr int N = 5;

void send_heartbeat() {
    int sequence = 1;

    while (true) {

        json heartbeat_event = make_heartbeat_event(sequence++);

        sink_tcp(heartbeat_event);

        std::this_thread::sleep_for(std::chrono::seconds(N));
    }
}
