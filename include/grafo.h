#ifndef GRAFO_H
#define GRAFO_H

#include "dados_identificados.h"
#include <stddef.h>

typedef struct Aresta
{
    int vizinho;
    int peso;
    struct Aresta *proxima;
} Aresta;

typedef struct Grafo
{
    size_t quantidade_vertices;
    Aresta **lista_adj;
} Grafo;

Grafo *grafo_criar(size_t quantidade_vertices);

void grafo_destruir(Grafo *grafo);

/* Adiciona ou atualiza uma aresta incrementando o peso se ela ja existir.
 * A aresta sera inserida de forma bidirecional (origem->destino e destino->origem). */
int grafo_adicionar_aresta(Grafo *grafo, int origem, int destino);

/* Adiciona ou atualiza uma aresta com incremento de peso informado. */
int grafo_adicionar_aresta_com_peso(
    Grafo *grafo,
    int origem,
    int destino,
    int peso);

/* Constroi o grafo a partir dos DadosIdentificados parseados. */
Grafo *grafo_construir_de_dados(
    const DadosIdentificados *dados,
    size_t quantidade_vertices);

#endif
