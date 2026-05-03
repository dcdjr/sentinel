#pragma once

#include <nlohmann/json.hpp>

/* The sink logic provides an abstraction where event logic
   doesn't need to be concerned about the flow of its data. */

void sink_stdout(const nlohmann::json& data);
