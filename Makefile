CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -Iinclude

BUILD_DIR = build

AGENT_TARGET = sentinel-agent
COLLECTOR_TARGET = sentinel-collector

AGENT_SRC = \
	src/agent/agent_main.cpp \
	src/agent/heartbeat.cpp \
	src/agent/sink.cpp \
	src/common/event.cpp

COLLECTOR_SRC = \
	src/collector/collector_main.cpp \
	src/collector/collector.cpp \
    src/collector/storage.cpp

AGENT_OBJ = $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(AGENT_SRC))
COLLECTOR_OBJ = $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(COLLECTOR_SRC))

TARGETS = $(AGENT_TARGET) $(COLLECTOR_TARGET)

all: $(TARGETS)

$(AGENT_TARGET): $(AGENT_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(COLLECTOR_TARGET): $(COLLECTOR_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGETS)

.PHONY: all clean
