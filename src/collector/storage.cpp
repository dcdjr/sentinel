#include <fstream>

#include "sentinel/collector/storage.hpp"

using json = nlohmann::json;

bool store_event(const nlohmann::json& event) {
    std::ofstream file("data/events.jsonl", std::ios::app);

    if (!file) return false;

    std::string event_str = event.dump();

    file << event_str << "\n";

    return true;
}
