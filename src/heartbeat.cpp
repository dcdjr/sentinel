#include "heartbeat.hpp"
#include "event.hpp"
#include <chrono>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>

using json = nlohmann::json;

constexpr int N = 5;

/* Heartbeat is meant to send system info and other important metadata to
   collector The heartbeat is sent every N seconds, is sent in structured JSON
   format, and will eventually send over TCP but for now it just sends to
   stdout. */

auto last_time = std::chrono::system_clock::now();

void send_heartbeat() {
  while (true) {

    json heartbeat_event = make_heartbeat_event();

    std::string heartbeat_event_str = heartbeat_event.dump();
    std::cout << heartbeat_event_str << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(N));
  }
}
