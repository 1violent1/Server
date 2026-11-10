CXX ?= g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lcryptopp

TARGET = vcalc_server
TEST_TARGET = unit_tests
BUILD_DIR = obj

CORE_SRCS = src/Logger.cpp \
            src/UserDatabase.cpp \
            src/Authenticator.cpp \
            src/DataProcessor.cpp \
            src/Communicator.cpp \
            src/Server.cpp

CORE_OBJS = $(patsubst src/%.cpp, $(BUILD_DIR)/%.o, $(CORE_SRCS))
SERVER_MAIN_OBJ = $(BUILD_DIR)/main.o

TEST_SRCS = tests/main.cpp \
            tests/test_DataProcessor.cpp \
            tests/test_Authenticator.cpp \
            tests/test_UserDatabase.cpp \
            tests/test_Logger.cpp \
            tests/test_Communicator.cpp

TEST_OBJS = $(patsubst tests/%.cpp, $(BUILD_DIR)/tests/%.o, $(TEST_SRCS))
TEST_LDFLAGS = -lcryptopp -lUnitTest++

all: $(TARGET)

$(TARGET): $(CORE_OBJS) $(SERVER_MAIN_OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(TEST_TARGET): $(CORE_OBJS) $(TEST_OBJS)
	$(CXX) $^ -o $@ $(TEST_LDFLAGS)

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/tests/%.o: tests/%.cpp | $(BUILD_DIR)/tests
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/tests:
	mkdir -p $(BUILD_DIR)/tests

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGET)

.PHONY: all clean test
