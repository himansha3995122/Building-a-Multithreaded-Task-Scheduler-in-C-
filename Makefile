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

# jthread demo build (separate source file, separate binary)
JTHREAD_TARGET = jthread_demo
JTHREAD_FLAGS  = -std=c++20 -Wall -Wextra -pthread -g
JTHREAD_SRC    = src/jthread_demo.cpp

# Bank account demo build (separate source file, separate binary)
BANK_ACCOUNT_TARGET = bank_account
BANK_ACCOUNT_SRC    = src/bank_account.cpp
BANK_ACCOUNT_FLAGS  = -std=c++20 -Wall -Wextra -pthread -g

SRC = src/main.cpp

.PHONY: all debug valgrind jthread jthread-valgrind bank_account bank_account-valgrind clean

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

# jthread demo: builds and runs src/jthread_demo.cpp
jthread: $(JTHREAD_TARGET)
	./$(JTHREAD_TARGET)

# jthread demo under valgrind/helgrind
jthread-valgrind: $(JTHREAD_TARGET)
	valgrind --tool=helgrind ./$(JTHREAD_TARGET)

$(JTHREAD_TARGET): $(JTHREAD_SRC)
	$(CXX) $(JTHREAD_FLAGS) -o $(JTHREAD_TARGET) $(JTHREAD_SRC)

# bank account demo: builds and runs src/bank_account.cpp
bank_account: $(BANK_ACCOUNT_TARGET)
	./$(BANK_ACCOUNT_TARGET)

# bank account demo under valgrind/helgrind
bank_account-valgrind: $(BANK_ACCOUNT_TARGET)
	valgrind --tool=helgrind ./$(BANK_ACCOUNT_TARGET)

$(BANK_ACCOUNT_TARGET): $(BANK_ACCOUNT_SRC)
	$(CXX) $(BANK_ACCOUNT_FLAGS) -o $(BANK_ACCOUNT_TARGET) $(BANK_ACCOUNT_SRC)


clean:
	rm -f $(RELEASE_TARGET) $(DEBUG_TARGET) $(VALGRIND_TARGET) $(JTHREAD_TARGET) $(BANK_ACCOUNT_TARGET)