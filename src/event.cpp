#include "event.hpp"

using json = nlohmann::json;

json make_heartbeat_event() {
  json heartbeat_event;

  // Get current time
  auto now = std::chrono::system_clock::now();
  std::time_t now_c = std::chrono::system_clock::to_time_t(now);
  char buf[100];
  std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now_c));
  std::string now_s(buf);

  // Populate JSON fields
  heartbeat_event["timestamp"] = now_s;
  heartbeat_event["CPU"] = "42%";
  heartbeat_event["Memory"] = "10000/16000 bytes";

  return heartbeat_event;
}
