CXX       = g++
CXXFLAGS  = -I /usr/local/include -I ./src/public -std=c++20
LDFLAGS   = -L/usr/local/lib -lraylib

SRC_DIR   = src
BUILD_DIR = bin

# Find all .cpp files under src/ (including src/public/ and deeper)
SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
# Convert each .cpp path into a corresponding .o path inside bin/
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

# Rule to build any .o file from its .cpp
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	bear -- $(CXX) $(CXXFLAGS) -c $< -o $@

# The final executable: depends on all .o files
run: $(OBJECTS)
	$(CXX) $^ -o $(BUILD_DIR)/game $(LDFLAGS)
	chmod +x $(BUILD_DIR)/game
	./$(BUILD_DIR)/game

clean:
	rm -rf $(BUILD_DIR)

.PHONY: run clean
