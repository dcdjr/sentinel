#include <chrono>
#include <nlohmann/json.hpp>
#include <thread>

#include "heartbeat.hpp"
#include "event.hpp"
#include "sink.hpp"

using json = nlohmann::json;

constexpr int N = 5;

void send_heartbeat() {
    while (true) {

        json heartbeat_event = make_heartbeat_event();

        sink_stdout(heartbeat_event);

        std::this_thread::sleep_for(std::chrono::seconds(N));
    }
}
