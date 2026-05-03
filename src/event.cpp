#include <chrono>
#include <ctime>
#include <string>

#include "event.hpp"

using json = nlohmann::json;

static json make_common_envelope(std::string event_type, json event_payload) {
    json common_envelope;

    // Get current time
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    char buf[100];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now_c));
    std::string now_s(buf);
    
    // Build common event format
    common_envelope["event_type"] = event_type;
    common_envelope["timestamp"] = now_s;
    common_envelope["agent_id"] = 1;
    common_envelope["hostname"] = "...";
    common_envelope["sequence"] = 2;
    common_envelope["payload"] = event_payload;

    return common_envelope;
}

json make_heartbeat_event() {
    // Build heartbeat-specific payload
    json heartbeat_payload;
    
    heartbeat_payload["cpu_percent"] = 0.99f;
    heartbeat_payload["memory_used_bytes"] = 1000;
    heartbeat_payload["memory_total_bytes"] = 1000000;
    heartbeat_payload["uptime_seconds"] = 69.1f;

    // Build common event format
    json heartbeat_event = make_common_envelope("heartbeat", heartbeat_payload);

    return heartbeat_event;
}
