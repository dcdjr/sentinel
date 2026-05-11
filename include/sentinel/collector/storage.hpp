#pragma once

#include <nlohmann/json.hpp>

bool store_event(const nlohmann::json& event);
