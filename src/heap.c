#include "heap.h"

#include <stdlib.h>

struct HeapAresta {
    ItemHeap *itens;
    size_t capacidade;
    size_t tamanho;
};

static void troca_itens(ItemHeap *a, ItemHeap *b)
{
    ItemHeap temporario = *a;
    *a = *b;
    *b = temporario;
}

static void subir(HeapAresta *heap, size_t indice)
{
    while (indice > 0) {
        size_t pai = (indice - 1) / 2;

        if (heap->itens[pai].peso >= heap->itens[indice].peso) {
            break;
        }

        troca_itens(&heap->itens[pai], &heap->itens[indice]);
        indice = pai;
    }
}

static void descer(HeapAresta *heap, size_t indice)
{
    for (;;) {
        size_t esquerda = 2 * indice + 1;
        size_t direita = 2 * indice + 2;
        size_t maior = indice;

        if (esquerda < heap->tamanho &&
            heap->itens[esquerda].peso > heap->itens[maior].peso) {
            maior = esquerda;
        }

        if (direita < heap->tamanho &&
            heap->itens[direita].peso > heap->itens[maior].peso) {
            maior = direita;
        }

        if (maior == indice) {
            break;
        }

        troca_itens(&heap->itens[indice], &heap->itens[maior]);
        indice = maior;
    }
}

static int aumentar_capacidade(HeapAresta *heap)
{
    size_t nova_capacidade = heap->capacidade * 2;
    ItemHeap *novos_itens = realloc(
        heap->itens,
        nova_capacidade * sizeof(*novos_itens)
    );

    if (novos_itens == NULL) {
        return 0;
    }

    heap->itens = novos_itens;
    heap->capacidade = nova_capacidade;
    return 1;
}

HeapAresta *heap_criar(size_t capacidade_inicial)
{
    HeapAresta *heap;

    if (capacidade_inicial == 0) {
        return NULL;
    }

    heap = malloc(sizeof(*heap));
    if (heap == NULL) {
        return NULL;
    }

    heap->itens = malloc(capacidade_inicial * sizeof(*heap->itens));
    if (heap->itens == NULL) {
        free(heap);
        return NULL;
    }

    heap->capacidade = capacidade_inicial;
    heap->tamanho = 0;
    return heap;
}

void heap_destruir(HeapAresta *heap)
{
    if (heap == NULL) {
        return;
    }

    free(heap->itens);
    free(heap);
}

int heap_inserir_aresta(HeapAresta *heap, int origem, int destino, int peso)
{
    if (heap == NULL) {
        return 0;
    }

    if (heap->tamanho == heap->capacidade && !aumentar_capacidade(heap)) {
        return 0;
    }

    heap->itens[heap->tamanho].origem = origem;
    heap->itens[heap->tamanho].destino = destino;
    heap->itens[heap->tamanho].peso = peso;

    subir(heap, heap->tamanho);
    heap->tamanho++;
    return 1;
}

int heap_remover_maximo(HeapAresta *heap, ItemHeap *item)
{
    if (heap == NULL || item == NULL || heap->tamanho == 0) {
        return 0;
    }

    *item = heap->itens[0];
    heap->tamanho--;

    if (heap->tamanho > 0) {
        heap->itens[0] = heap->itens[heap->tamanho];
        descer(heap, 0);
    }

    return 1;
}

int heap_vazio(const HeapAresta *heap)
{
    return heap == NULL || heap->tamanho == 0;
}

size_t heap_tamanho(const HeapAresta *heap)
{
    return heap == NULL ? 0 : heap->tamanho;
}

HeapAresta *heap_construir_de_grafo(const Grafo *grafo)
{
    HeapAresta *heap;
    size_t origem;

    if (grafo == NULL || grafo->quantidade_vertices == 0) {
        return NULL;
    }

    heap = heap_criar(grafo->quantidade_vertices);
    if (heap == NULL) {
        return NULL;
    }

    for (origem = 0; origem < grafo->quantidade_vertices; origem++) {
        Aresta *atual = grafo->lista_adj[origem];

        while (atual != NULL) {
            /* Cada aresta nao direcionada aparece duas vezes na lista
             * de adjacencia (origem->destino e destino->origem).
             * Inserimos apenas uma vez, quando origem < destino. */
            if ((int)origem < atual->vizinho) {
                if (!heap_inserir_aresta(
                        heap,
                        (int)origem,
                        atual->vizinho,
                        atual->peso
                    )) {
                    heap_destruir(heap);
                    return NULL;
                }
            }

            atual = atual->proxima;
        }
    }

    return heap;
}
