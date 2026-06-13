#ifndef HEAP_H
#define HEAP_H

#include "grafo.h"

#include <stddef.h>

typedef struct {
    int origem;
    int destino;
    int peso;
} ItemHeap;

typedef struct HeapAresta HeapAresta;

HeapAresta *heap_criar(size_t capacidade_inicial);

void heap_destruir(HeapAresta *heap);

int heap_inserir_aresta(HeapAresta *heap, int origem, int destino, int peso);

int heap_remover_maximo(HeapAresta *heap, ItemHeap *item);

int heap_vazio(const HeapAresta *heap);

size_t heap_tamanho(const HeapAresta *heap);

HeapAresta *heap_construir_de_grafo(const Grafo *grafo);

#endif
