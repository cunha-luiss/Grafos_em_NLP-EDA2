#include "grafo.h"
#include "kruskal.h"

#include <assert.h>
#include <stdio.h>

static void adicionar_aresta_ponderada(Grafo *grafo, int origem, int destino, int peso)
{
    int i;

    for (i = 0; i < peso; i++)
    {
        assert(grafo_adicionar_aresta(grafo, origem, destino));
    }
}

static int peso_da_aresta(const Grafo *grafo, int origem, int destino)
{
    Aresta *atual;

    assert(grafo != NULL);
    assert(origem >= 0 && destino >= 0);
    assert((size_t)origem < grafo->quantidade_vertices);
    assert((size_t)destino < grafo->quantidade_vertices);

    atual = grafo->lista_adj[origem];
    while (atual != NULL)
    {
        if (atual->vizinho == destino)
        {
            return atual->peso;
        }
        atual = atual->proxima;
    }

    return 0;
}

static size_t contar_arestas_unicas(const Grafo *grafo)
{
    size_t origem;
    size_t total = 0;

    assert(grafo != NULL);

    for (origem = 0; origem < grafo->quantidade_vertices; origem++)
    {
        Aresta *atual = grafo->lista_adj[origem];

        while (atual != NULL)
        {
            if ((int)origem < atual->vizinho)
            {
                total++;
            }
            atual = atual->proxima;
        }
    }

    return total;
}

static void testar_agm_maxima_simples(void)
{
    Grafo *grafo = grafo_criar(4);
    Grafo *agm;

    assert(grafo != NULL);

    adicionar_aresta_ponderada(grafo, 0, 1, 5);
    adicionar_aresta_ponderada(grafo, 0, 2, 4);
    adicionar_aresta_ponderada(grafo, 1, 2, 3);
    adicionar_aresta_ponderada(grafo, 1, 3, 2);
    adicionar_aresta_ponderada(grafo, 2, 3, 1);

    agm = kruskal_construir_agm_maxima(grafo);
    assert(agm != NULL);
    assert(contar_arestas_unicas(agm) == 3);
    assert(peso_da_aresta(agm, 0, 1) == 5);
    assert(peso_da_aresta(agm, 0, 2) == 4);
    assert(peso_da_aresta(agm, 1, 3) == 2);
    assert(peso_da_aresta(agm, 1, 2) == 0);
    assert(peso_da_aresta(agm, 2, 3) == 0);

    grafo_destruir(agm);
    grafo_destruir(grafo);
}

static void testar_entradas_invalidas(void)
{
    assert(kruskal_construir_agm_maxima(NULL) == NULL);
}

int main(void)
{
    testar_agm_maxima_simples();
    testar_entradas_invalidas();

    puts("Todos os testes do Kruskal passaram.");
    return 0;
}