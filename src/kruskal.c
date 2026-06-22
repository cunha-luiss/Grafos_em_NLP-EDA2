#include "kruskal.h"

#include "heap.h"
#include "union_find.h"

static void destruir_recursos(
    HeapAresta *heap,
    UnionFind *union_find,
    Grafo *grafo)
{
    heap_destruir(heap);
    union_find_destruir(union_find);
    grafo_destruir(grafo);
}

Grafo *kruskal_construir_agm_maxima(const Grafo *grafo)
{
    HeapAresta *heap;
    UnionFind *union_find;
    Grafo *arvore_geradora_maxima;
    size_t arestas_selecionadas = 0;

    if (grafo == NULL || grafo->quantidade_vertices == 0)
    {
        return NULL;
    }

    heap = heap_construir_de_grafo(grafo);
    if (heap == NULL)
    {
        return NULL;
    }

    union_find = union_find_criar(grafo->quantidade_vertices);
    if (union_find == NULL)
    {
        heap_destruir(heap);
        return NULL;
    }

    arvore_geradora_maxima = grafo_criar(grafo->quantidade_vertices);
    if (arvore_geradora_maxima == NULL)
    {
        destruir_recursos(heap, union_find, NULL);
        return NULL;
    }

    while (
        arestas_selecionadas < grafo->quantidade_vertices - 1 && !heap_vazio(heap))
    {
        ItemHeap item;

        if (!heap_remover_maximo(heap, &item))
        {
            break;
        }

        if (union_find_unir(union_find, item.origem, item.destino))
        {
            if (!grafo_adicionar_aresta_com_peso(
                    arvore_geradora_maxima,
                    item.origem,
                    item.destino,
                    item.peso))
            {
                destruir_recursos(heap, union_find, arvore_geradora_maxima);
                return NULL;
            }

            arestas_selecionadas++;
        }
    }

    destruir_recursos(heap, union_find, NULL);
    return arvore_geradora_maxima;
}

Grafo *kruskal_construir_floresta(const Grafo *grafo, int limiar_peso)
{
    HeapAresta *heap;
    UnionFind *union_find;
    Grafo *floresta;

    if (grafo == NULL || grafo->quantidade_vertices == 0)
    {
        return NULL;
    }

    heap = heap_construir_de_grafo(grafo);
    if (heap == NULL)
    {
        return NULL;
    }

    union_find = union_find_criar(grafo->quantidade_vertices);
    if (union_find == NULL)
    {
        heap_destruir(heap);
        return NULL;
    }

    floresta = grafo_criar(grafo->quantidade_vertices);
    if (floresta == NULL)
    {
        destruir_recursos(heap, union_find, NULL);
        return NULL;
    }

    while (!heap_vazio(heap))
    {
        ItemHeap item;

        if (!heap_remover_maximo(heap, &item))
        {
            break;
        }

        if (item.peso < limiar_peso)
        {
            break;
        }

        if (union_find_unir(union_find, item.origem, item.destino))
        {
            if (!grafo_adicionar_aresta_com_peso(
                    floresta,
                    item.origem,
                    item.destino,
                    item.peso))
            {
                destruir_recursos(heap, union_find, floresta);
                return NULL;
            }
        }
    }

    destruir_recursos(heap, union_find, NULL);
    return floresta;
}
