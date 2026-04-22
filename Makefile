CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -Iinclude

TARGET = sentinel
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp,src/%.o,$(SRC))

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: clean
