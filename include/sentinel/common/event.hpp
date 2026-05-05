#pragma once

#include <nlohmann/json.hpp>

// Functions that help build each event

nlohmann::json make_heartbeat_event();
