CC := gcc
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Iinclude
TARGET := build/grafos_nlp.exe
SOURCES := $(wildcard src/*.c)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCES) | build
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

build:
	mkdir -p build

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
