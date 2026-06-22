#include "grafo.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

static int vetor_contem_int(const int *valores, size_t quantidade, int valor)
{
    size_t i;

    for (i = 0; i < quantidade; i++)
    {
        if (valores[i] == valor)
        {
            return 1;
        }
    }

    return 0;
}

static void destruir_vetores_int(int **vetores, size_t quantidade)
{
    size_t i;

    if (vetores == NULL)
    {
        return;
    }

    for (i = 0; i < quantidade; i++)
    {
        free(vetores[i]);
    }

    free(vetores);
}

static int vertice_tem_aresta(const Grafo *grafo, int vertice)
{
    if (
        grafo == NULL ||
        vertice < 0 ||
        (size_t)vertice >= grafo->quantidade_vertices)
    {
        return 0;
    }

    return grafo->lista_adj[vertice] != NULL;
}

static int contar_caracteristicas_em_comum(
    const int *caracteristicas_a,
    size_t quantidade_a,
    const int *caracteristicas_b,
    size_t quantidade_b)
{
    size_t i, j;
    int quantidade = 0;

    for (i = 0; i < quantidade_a; i++)
    {
        for (j = 0; j < quantidade_b; j++)
        {
            if (caracteristicas_a[i] == caracteristicas_b[j])
            {
                quantidade++;
                break;
            }
        }
    }

    return quantidade;
}

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

Grafo *grafo_construir_por_caracteristicas_em_comum(
    const DadosIdentificados *dados,
    size_t quantidade_vertices,
    int minimo_caracteristicas_em_comum)
{
    Grafo *grafo;
    int **caracteristicas_unicas;
    size_t *quantidades_unicas;
    size_t i, j;

    if (
        dados == NULL ||
        quantidade_vertices == 0 ||
        minimo_caracteristicas_em_comum <= 0)
    {
        return NULL;
    }

    caracteristicas_unicas = calloc(
        dados->quantidade_esportes,
        sizeof(*caracteristicas_unicas));
    quantidades_unicas = calloc(
        dados->quantidade_esportes,
        sizeof(*quantidades_unicas));

    if (caracteristicas_unicas == NULL || quantidades_unicas == NULL)
    {
        destruir_vetores_int(caracteristicas_unicas, dados->quantidade_esportes);
        free(quantidades_unicas);
        return NULL;
    }

    for (i = 0; i < dados->quantidade_esportes; i++)
    {
        const EsporteIdentificado *esporte = &dados->esportes[i];

        if (esporte->quantidade_caracteristicas == 0)
        {
            continue;
        }

        if (
            esporte->quantidade_caracteristicas >
            SIZE_MAX / sizeof(*caracteristicas_unicas[i]))
        {
            destruir_vetores_int(caracteristicas_unicas, dados->quantidade_esportes);
            free(quantidades_unicas);
            return NULL;
        }

        caracteristicas_unicas[i] = malloc(
            esporte->quantidade_caracteristicas *
            sizeof(*caracteristicas_unicas[i]));
        if (caracteristicas_unicas[i] == NULL)
        {
            destruir_vetores_int(caracteristicas_unicas, dados->quantidade_esportes);
            free(quantidades_unicas);
            return NULL;
        }

        for (j = 0; j < esporte->quantidade_caracteristicas; j++)
        {
            int id = esporte->ids_caracteristicas[j];

            if (id < 0 || (size_t)id >= quantidade_vertices)
            {
                destruir_vetores_int(caracteristicas_unicas, dados->quantidade_esportes);
                free(quantidades_unicas);
                return NULL;
            }

            if (!vetor_contem_int(
                    caracteristicas_unicas[i],
                    quantidades_unicas[i],
                    id))
            {
                caracteristicas_unicas[i][quantidades_unicas[i]++] = id;
            }
        }
    }

    grafo = grafo_criar(quantidade_vertices);
    if (grafo == NULL)
    {
        destruir_vetores_int(caracteristicas_unicas, dados->quantidade_esportes);
        free(quantidades_unicas);
        return NULL;
    }

    for (i = 0; i < dados->quantidade_esportes; i++)
    {
        const EsporteIdentificado *esporte_i = &dados->esportes[i];

        for (j = i + 1; j < dados->quantidade_esportes; j++)
        {
            const EsporteIdentificado *esporte_j = &dados->esportes[j];
            int caracteristicas_em_comum =
                contar_caracteristicas_em_comum(
                    caracteristicas_unicas[i],
                    quantidades_unicas[i],
                    caracteristicas_unicas[j],
                    quantidades_unicas[j]);

            if (caracteristicas_em_comum >= minimo_caracteristicas_em_comum)
            {
                if (!grafo_adicionar_aresta_com_peso(
                        grafo,
                        esporte_i->id_esporte,
                        esporte_j->id_esporte,
                        caracteristicas_em_comum))
                {
                    grafo_destruir(grafo);
                    destruir_vetores_int(
                        caracteristicas_unicas,
                        dados->quantidade_esportes);
                    free(quantidades_unicas);
                    return NULL;
                }
            }
        }
    }

    if (minimo_caracteristicas_em_comum > 1)
    {
        int minimo_para_resgate = minimo_caracteristicas_em_comum - 1;

        for (i = 0; i < dados->quantidade_esportes; i++)
        {
            const EsporteIdentificado *esporte_i = &dados->esportes[i];
            int melhor_esporte = -1;
            int melhor_peso = 0;

            if (vertice_tem_aresta(grafo, esporte_i->id_esporte))
            {
                continue;
            }

            for (j = 0; j < dados->quantidade_esportes; j++)
            {
                int caracteristicas_em_comum;

                if (i == j)
                {
                    continue;
                }

                caracteristicas_em_comum =
                    contar_caracteristicas_em_comum(
                        caracteristicas_unicas[i],
                        quantidades_unicas[i],
                        caracteristicas_unicas[j],
                        quantidades_unicas[j]);

                if (caracteristicas_em_comum > melhor_peso)
                {
                    melhor_peso = caracteristicas_em_comum;
                    melhor_esporte = dados->esportes[j].id_esporte;
                }
            }

            if (melhor_peso >= minimo_para_resgate)
            {
                if (!grafo_adicionar_aresta_com_peso(
                        grafo,
                        esporte_i->id_esporte,
                        melhor_esporte,
                        melhor_peso))
                {
                    grafo_destruir(grafo);
                    destruir_vetores_int(
                        caracteristicas_unicas,
                        dados->quantidade_esportes);
                    free(quantidades_unicas);
                    return NULL;
                }
            }
        }
    }

    destruir_vetores_int(caracteristicas_unicas, dados->quantidade_esportes);
    free(quantidades_unicas);
    return grafo;
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
