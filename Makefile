CXX ?= g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lcryptopp

TARGET = vcalc_server
TEST_TARGET = tests
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

functional_test: $(TARGET)
	./functional_test.sh
doc:
	doxygen Doxyfile
	@if [ -d "docs/latex" ] && command -v pdflatex >/dev/null 2>&1; then \
	  echo "Генерация PDF документации из LaTeX..."; \
	  $(MAKE) -C docs/latex pdf; \
	  cp docs/latex/refman.pdf docs/vcalc_server_documentation.pdf; \
	  echo "============================================================"; \
	  echo "PDF документация успешно сохранена в:"; \
	  echo "  docs/vcalc_server_documentation.pdf"; \
	  echo "============================================================"; \
	else \
	  echo "HTML документация создана в docs/html/index.html"; \
	  echo "Для сборки PDF требуется установить pdflatex (пакет doxygen-latex или texlive)"; \
	fi

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGET) docs

.PHONY: all clean test functional_test doc
