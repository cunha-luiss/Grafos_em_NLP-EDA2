#include "grafo.h"
#include <stdlib.h>
#include <stdio.h>

Grafo *grafo_criar(size_t quantidade_vertices)
{
    Grafo *g = malloc(sizeof(Grafo));
    if (g == NULL)
        return NULL;

    g->quantidade_vertices = quantidade_vertices;
    g->lista_adj = calloc(quantidade_vertices, sizeof(Aresta *));

    if (g->lista_adj == NULL)
    {
        free(g);
        return NULL;
    }

    return g;
}

void grafo_destruir(Grafo *grafo)
{
    size_t i;
    Aresta *atual, *temp;

    if (grafo == NULL)
        return;

    if (grafo->lista_adj != NULL)
    {
        for (i = 0; i < grafo->quantidade_vertices; i++)
        {
            atual = grafo->lista_adj[i];
            while (atual != NULL)
            {
                temp = atual;
                atual = atual->proxima;
                free(temp);
            }
        }
        free(grafo->lista_adj);
    }
    free(grafo);
}

static int adicionar_aresta_direcionada_com_peso(
    Grafo *grafo,
    int origem,
    int destino,
    int peso)
{
    Aresta *atual = grafo->lista_adj[origem];

    /* Procurar se ja existe e incrementar o peso */
    while (atual != NULL)
    {
        if (atual->vizinho == destino)
        {
            atual->peso += peso;
            return 1;
        }
        atual = atual->proxima;
    }

    /* Se nao encontrou, criar nova aresta com peso 1 */
    Aresta *nova = malloc(sizeof(Aresta));
    if (nova == NULL)
        return 0;

    nova->vizinho = destino;
    nova->peso = peso;
    nova->proxima = grafo->lista_adj[origem];
    grafo->lista_adj[origem] = nova;

    return 1;
}

int grafo_adicionar_aresta(Grafo *grafo, int origem, int destino)
{
    return grafo_adicionar_aresta_com_peso(grafo, origem, destino, 1);
}

int grafo_adicionar_aresta_com_peso(
    Grafo *grafo,
    int origem,
    int destino,
    int peso)
{
    if (grafo == NULL || origem < 0 || destino < 0)
        return 0;
    if (peso <= 0)
        return 0;
    if ((size_t)origem >= grafo->quantidade_vertices || (size_t)destino >= grafo->quantidade_vertices)
        return 0;

    if (!adicionar_aresta_direcionada_com_peso(grafo, origem, destino, peso))
        return 0;
    if (!adicionar_aresta_direcionada_com_peso(grafo, destino, origem, peso))
        return 0;

    return 1;
}

void grafo_adicionar_arestas_char_char(
    Grafo *grafo,
    const DadosIdentificados *dados)
{
    size_t i, j, k;

    if (grafo == NULL || dados == NULL)
    {
        return;
    }

    for (i = 0; i < dados->quantidade_esportes; i++)
    {
        const EsporteIdentificado *esporte = &dados->esportes[i];
        if (esporte->quantidade_caracteristicas == 0)
        {
            continue;
        }

        /* Allocating temporary array to find unique IDs for the current sport */
        int *unicos = malloc(esporte->quantidade_caracteristicas * sizeof(int));
        if (unicos == NULL)
        {
            continue;
        }
        size_t qtd_unicos = 0;

        for (j = 0; j < esporte->quantidade_caracteristicas; j++)
        {
            int id = esporte->ids_caracteristicas[j];
            int ja_existe = 0;
            for (k = 0; k < qtd_unicos; k++)
            {
                if (unicos[k] == id)
                {
                    ja_existe = 1;
                    break;
                }
            }
            if (!ja_existe)
            {
                unicos[qtd_unicos++] = id;
            }
        }

        for (j = 0; j < qtd_unicos; j++)
        {
            for (k = j + 1; k < qtd_unicos; k++)
            {
                int ci = unicos[j];
                int ck = unicos[k];

                /* peso acumula automaticamente se aresta ja existir */
                grafo_adicionar_aresta_com_peso(grafo, ci, ck, 1);
            }
        }

        free(unicos);
    }
}

Grafo *grafo_construir_de_dados(const DadosIdentificados *dados, size_t quantidade_vertices)
{
    Grafo *g;
    size_t i, j;

    if (dados == NULL || quantidade_vertices == 0)
        return NULL;

    g = grafo_criar(quantidade_vertices);
    if (g == NULL)
        return NULL;

    for (i = 0; i < dados->quantidade_esportes; i++)
    {
        EsporteIdentificado *esporte = &dados->esportes[i];
        for (j = 0; j < esporte->quantidade_caracteristicas; j++)
        {
            int id_caracteristica = esporte->ids_caracteristicas[j];

            if (!grafo_adicionar_aresta(g, esporte->id_esporte, id_caracteristica))
            {
                grafo_destruir(g);
                return NULL;
            }
        }
    }

    return g;
}
