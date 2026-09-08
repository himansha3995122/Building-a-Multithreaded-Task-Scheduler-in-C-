CXX = g++

# Release build settings
RELEASE_TARGET = scheduler
RELEASE_FLAGS  = -std=c++20 -Wall -Wextra -O2 -pthread

# Debug / ThreadSanitizer build settings
DEBUG_TARGET = main
DEBUG_FLAGS  = -std=c++20 -Wall -Wextra -pthread -fsanitize=thread -g
DEBUG_LDFLAGS = -pthread -fsanitize=thread

# Debug build for valgrind (no sanitizer instrumentation - valgrind needs a plain binary)
VALGRIND_TARGET = main_vg
VALGRIND_FLAGS  = -std=c++20 -Wall -Wextra -pthread -g -O0

SRC = src/main.cpp

.PHONY: all debug valgrind clean

# Default: release build
all: $(RELEASE_TARGET)

$(RELEASE_TARGET): $(SRC)
	$(CXX) $(RELEASE_FLAGS) -o $(RELEASE_TARGET) $(SRC)

# ThreadSanitizer debug build
debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(SRC)
	$(CXX) $(DEBUG_FLAGS) -o $(DEBUG_TARGET) $(SRC) $(DEBUG_LDFLAGS)

# Valgrind-friendly debug build (no sanitizer - they conflict with valgrind)
valgrind: $(VALGRIND_TARGET)
	valgrind --tool=helgrind ./$(VALGRIND_TARGET)

$(VALGRIND_TARGET): $(SRC)
	$(CXX) $(VALGRIND_FLAGS) -o $(VALGRIND_TARGET) $(SRC)

clean:
	rm -f $(RELEASE_TARGET) $(DEBUG_TARGET) $(VALGRIND_TARGET)