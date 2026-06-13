CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Iinclude
TARGET := build/grafos_nlp.exe
SOURCES := $(wildcard src/*.c)
HASH_TEST_TARGET := build/test_tabela_hash.exe
HASH_TEST_SOURCES := tests/test_tabela_hash.c src/tabela_hash.c
CATALOG_TEST_TARGET := build/test_catalogo.exe
CATALOG_TEST_SOURCES := tests/test_catalogo.c src/catalogo.c src/tabela_hash.c
FILE_TEST_TARGET := build/test_leitor_arquivo.exe
FILE_TEST_SOURCES := tests/test_leitor_arquivo.c src/leitor_arquivo.c
PARSER_TEST_TARGET := build/test_parser_json.exe
PARSER_TEST_SOURCES := tests/test_parser_json.c src/parser_json.c
SPORTS_TEST_TARGET := build/test_dados_esportes.exe
SPORTS_TEST_SOURCES := tests/test_dados_esportes.c src/dados_esportes.c \
	src/parser_json.c src/leitor_arquivo.c
IDS_TEST_TARGET := build/test_dados_identificados.exe
IDS_TEST_SOURCES := tests/test_dados_identificados.c \
	src/dados_identificados.c src/dados_esportes.c src/catalogo.c \
	src/tabela_hash.c src/parser_json.c src/leitor_arquivo.c
APP_TEST_TARGET := build/test_aplicacao.exe
APP_TEST_SOURCES := tests/test_aplicacao.c src/aplicacao.c \
	src/dados_identificados.c src/dados_esportes.c src/catalogo.c \
	src/tabela_hash.c src/parser_json.c src/leitor_arquivo.c \
	src/grafo.c src/heap.c
HEAP_TEST_TARGET := build/test_heap.exe
HEAP_TEST_SOURCES := tests/test_heap.c src/heap.c src/grafo.c

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

$(FILE_TEST_TARGET): $(FILE_TEST_SOURCES) | build
	$(CC) $(CFLAGS) $(FILE_TEST_SOURCES) -o $(FILE_TEST_TARGET)

$(PARSER_TEST_TARGET): $(PARSER_TEST_SOURCES) | build
	$(CC) $(CFLAGS) $(PARSER_TEST_SOURCES) -o $(PARSER_TEST_TARGET)

$(SPORTS_TEST_TARGET): $(SPORTS_TEST_SOURCES) | build
	$(CC) $(CFLAGS) $(SPORTS_TEST_SOURCES) -o $(SPORTS_TEST_TARGET)

$(IDS_TEST_TARGET): $(IDS_TEST_SOURCES) | build
	$(CC) $(CFLAGS) $(IDS_TEST_SOURCES) -o $(IDS_TEST_TARGET)

$(APP_TEST_TARGET): $(APP_TEST_SOURCES) | build
	$(CC) $(CFLAGS) $(APP_TEST_SOURCES) -o $(APP_TEST_TARGET)

$(HEAP_TEST_TARGET): $(HEAP_TEST_SOURCES) | build
	$(CC) $(CFLAGS) $(HEAP_TEST_SOURCES) -o $(HEAP_TEST_TARGET)

test: $(HASH_TEST_TARGET) $(CATALOG_TEST_TARGET) $(FILE_TEST_TARGET) \
		$(PARSER_TEST_TARGET) $(SPORTS_TEST_TARGET) $(IDS_TEST_TARGET) \
		$(APP_TEST_TARGET) $(HEAP_TEST_TARGET)
	./$(HASH_TEST_TARGET)
	./$(CATALOG_TEST_TARGET)
	./$(FILE_TEST_TARGET)
	./$(PARSER_TEST_TARGET)
	./$(SPORTS_TEST_TARGET)
	./$(IDS_TEST_TARGET)
	./$(APP_TEST_TARGET)
	./$(HEAP_TEST_TARGET)

clean:
	rm -f $(TARGET) $(HASH_TEST_TARGET) $(CATALOG_TEST_TARGET) \
		$(FILE_TEST_TARGET) $(PARSER_TEST_TARGET) $(SPORTS_TEST_TARGET) \
		$(IDS_TEST_TARGET) $(APP_TEST_TARGET) $(HEAP_TEST_TARGET)
