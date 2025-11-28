# Makefile for GM1 Sound Quality Module Tests
# 
# Implements: #540 (REQ-F-GM1-004: GM1 Sound Quality and Timbre Standards)
# 
# Usage:
#   make          - Build and run tests
#   make test     - Run tests only
#   make clean    - Clean build artifacts

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g
CFLAGS += -I../../src
LDFLAGS = -lm

# Directories
SRC_DIR = ../../src/gm1
TEST_DIR = .
BUILD_DIR = build

# Source files
SRC = $(SRC_DIR)/gm1_sound_quality.c
TEST_SRC = $(TEST_DIR)/test_gm1_sound_quality.c

# Output
TEST_BIN = $(BUILD_DIR)/test_gm1_sound_quality

.PHONY: all test clean

all: $(TEST_BIN) test

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TEST_BIN): $(SRC) $(TEST_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(TEST_SRC) $(LDFLAGS)

test: $(TEST_BIN)
	@echo "Running GM1 Sound Quality Tests..."
	@./$(TEST_BIN)

clean:
	rm -rf $(BUILD_DIR)
