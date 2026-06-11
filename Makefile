CC := gcc
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Iinclude
TARGET := build/grafos_nlp.exe
SOURCES := $(wildcard src/*.c)
HASH_TEST_TARGET := build/test_tabela_hash.exe
HASH_TEST_SOURCES := tests/test_tabela_hash.c src/tabela_hash.c
CATALOG_TEST_TARGET := build/test_catalogo.exe
CATALOG_TEST_SOURCES := tests/test_catalogo.c src/catalogo.c src/tabela_hash.c

.PHONY: all run test clean

all: $(TARGET)

$(TARGET): $(SOURCES) | build
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

build:
	mkdir -p build

run: all
	./$(TARGET)

$(HASH_TEST_TARGET): $(HASH_TEST_SOURCES) | build
	$(CC) $(CFLAGS) $(HASH_TEST_SOURCES) -o $(HASH_TEST_TARGET)

$(CATALOG_TEST_TARGET): $(CATALOG_TEST_SOURCES) | build
	$(CC) $(CFLAGS) $(CATALOG_TEST_SOURCES) -o $(CATALOG_TEST_TARGET)

test: $(HASH_TEST_TARGET) $(CATALOG_TEST_TARGET)
	./$(HASH_TEST_TARGET)
	./$(CATALOG_TEST_TARGET)

clean:
	rm -f $(TARGET) $(HASH_TEST_TARGET) $(CATALOG_TEST_TARGET)
