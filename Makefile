CXX ?= g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lcryptopp

TARGET = vcalc_server
BUILD_DIR = obj

SRCS = src/Logger.cpp \
       src/UserDatabase.cpp \
       src/Authenticator.cpp \
       src/DataProcessor.cpp \
       src/Communicator.cpp \
       src/Server.cpp \
       src/main.cpp

OBJS = $(patsubst src/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
