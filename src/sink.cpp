#include <iostream>

#include "sink.hpp"

using json = nlohmann::json;

void sink_stdout(const json& data) {
    std::string data_str = data.dump();
    std::cout << data_str << std::endl;
}
