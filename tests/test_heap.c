#include "heap.h"
#include "grafo.h"

#include <assert.h>
#include <stdio.h>

static void testar_criacao(void)
{
    HeapAresta *heap = heap_criar(4);

    assert(heap != NULL);
    assert(heap_vazio(heap));
    assert(heap_tamanho(heap) == 0);
    assert(heap_criar(0) == NULL);

    heap_destruir(heap);
    heap_destruir(NULL);
}

static void testar_insercao_e_ordem_de_remocao(void)
{
    HeapAresta *heap = heap_criar(2);
    ItemHeap item;

    assert(heap_inserir_aresta(heap, 0, 1, 3));
    assert(heap_inserir_aresta(heap, 1, 2, 7));
    assert(heap_inserir_aresta(heap, 2, 3, 1));
    assert(heap_inserir_aresta(heap, 3, 4, 5));
    assert(heap_tamanho(heap) == 4);

    /* O item de maior peso (7) deve ser o primeiro a ser removido. */
    assert(heap_remover_maximo(heap, &item));
    assert(item.peso == 7);
    assert(item.origem == 1 && item.destino == 2);

    assert(heap_remover_maximo(heap, &item));
    assert(item.peso == 5);

    assert(heap_remover_maximo(heap, &item));
    assert(item.peso == 3);

    assert(heap_remover_maximo(heap, &item));
    assert(item.peso == 1);

    assert(heap_vazio(heap));
    assert(!heap_remover_maximo(heap, &item));

    heap_destruir(heap);
}

static void testar_pesos_repetidos(void)
{
    HeapAresta *heap = heap_criar(4);
    ItemHeap item;
    int total = 0;

    assert(heap_inserir_aresta(heap, 0, 1, 5));
    assert(heap_inserir_aresta(heap, 1, 2, 5));
    assert(heap_inserir_aresta(heap, 2, 3, 5));

    while (heap_remover_maximo(heap, &item)) {
        assert(item.peso == 5);
        total++;
    }
    assert(total == 3);

    heap_destruir(heap);
}

static void testar_entradas_invalidas(void)
{
    HeapAresta *heap = heap_criar(2);
    ItemHeap item;

    assert(!heap_inserir_aresta(NULL, 0, 1, 1));
    assert(!heap_remover_maximo(NULL, &item));
    assert(!heap_remover_maximo(heap, NULL));
    assert(heap_vazio(NULL));
    assert(heap_tamanho(NULL) == 0);

    heap_destruir(heap);
}

static void testar_construcao_a_partir_de_grafo(void)
{
    Grafo *grafo = grafo_criar(4);
    HeapAresta *heap;
    ItemHeap item;

    assert(grafo != NULL);

    /* (0,1) selecionados juntos 2 vezes, (1,2) 1 vez, (0,2) 3 vezes. */
    assert(grafo_adicionar_aresta(grafo, 0, 1));
    assert(grafo_adicionar_aresta(grafo, 0, 1));
    assert(grafo_adicionar_aresta(grafo, 1, 2));
    assert(grafo_adicionar_aresta(grafo, 0, 2));
    assert(grafo_adicionar_aresta(grafo, 0, 2));
    assert(grafo_adicionar_aresta(grafo, 0, 2));

    heap = heap_construir_de_grafo(grafo);
    assert(heap != NULL);
    /* Cada aresta nao direcionada deve aparecer apenas uma vez. */
    assert(heap_tamanho(heap) == 3);

    assert(heap_remover_maximo(heap, &item));
    assert(item.peso == 3);
    assert(item.origem == 0 && item.destino == 2);

    assert(heap_remover_maximo(heap, &item));
    assert(item.peso == 2);
    assert(item.origem == 0 && item.destino == 1);

    assert(heap_remover_maximo(heap, &item));
    assert(item.peso == 1);
    assert(item.origem == 1 && item.destino == 2);

    heap_destruir(heap);
    grafo_destruir(grafo);

    assert(heap_construir_de_grafo(NULL) == NULL);
}

int main(void)
{
    testar_criacao();
    testar_insercao_e_ordem_de_remocao();
    testar_pesos_repetidos();
    testar_entradas_invalidas();
    testar_construcao_a_partir_de_grafo();

    puts("Todos os testes da heap passaram.");
    return 0;
}
