CC := gcc
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Iinclude
TARGET := build/grafos_nlp.exe
SOURCES := $(wildcard src/*.c)
TEST_TARGET := build/test_tabela_hash.exe
TEST_SOURCES := tests/test_tabela_hash.c src/tabela_hash.c

.PHONY: all run test clean

all: $(TARGET)

$(TARGET): $(SOURCES) | build
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

build:
	mkdir -p build

run: all
	./$(TARGET)

$(TEST_TARGET): $(TEST_SOURCES) | build
	$(CC) $(CFLAGS) $(TEST_SOURCES) -o $(TEST_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)
