#ifndef HEAP_H
#define HEAP_H

#include "grafo.h"

#include <stddef.h>

/* Representa uma aresta (origem, destino) com seu peso, isto e,
 * a quantidade de vezes que os dois nos foram selecionados juntos
 * durante a navegacao na Wikipedia. */
typedef struct {
    int origem;
    int destino;
    int peso;
} ItemHeap;

typedef struct HeapAresta HeapAresta;

/* Cria uma heap de maximo vazia, com a capacidade inicial informada.
 * Retorna NULL em caso de falha de alocacao ou capacidade igual a 0. */
HeapAresta *heap_criar(size_t capacidade_inicial);

void heap_destruir(HeapAresta *heap);

/* Insere uma aresta na heap, organizando-a de forma que o item de
 * maior peso fique sempre na raiz. Retorna 1 em sucesso e 0 em
 * caso de falha (heap NULL ou falha de alocacao). */
int heap_inserir_aresta(HeapAresta *heap, int origem, int destino, int peso);

/* Remove e devolve, em *item, a aresta de maior peso da heap.
 * Retorna 1 em sucesso e 0 se a heap estiver vazia ou for NULL. */
int heap_remover_maximo(HeapAresta *heap, ItemHeap *item);

int heap_vazio(const HeapAresta *heap);

size_t heap_tamanho(const HeapAresta *heap);

/* Constroi uma heap de maximo a partir de todas as arestas do grafo.
 * Cada aresta nao direcionada e inserida uma unica vez, usando o
 * peso (quantidade de selecoes em conjunto na Wikipedia) presente
 * no grafo. Retorna NULL em caso de falha. */
HeapAresta *heap_construir_de_grafo(const Grafo *grafo);

#endif
