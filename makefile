# Compiler and flags
CXX := g++
CXXFLAGS := -O2 -std=c++17
DEPFLAGS := -lssl -lcrypto

# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# Files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
TARGET := $(BIN_DIR)/app

# Default target
all: $(TARGET)

# Link objects to create the executable
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DEPFLAGS)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(DEPFLAGS)

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean

