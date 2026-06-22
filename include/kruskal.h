#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "grafo.h"

Grafo *kruskal_construir_agm_maxima(const Grafo *grafo);

/* Constroi uma floresta geradora maxima ignorando arestas com
 * peso < limiar_peso. Vertices sem arestas validas ficam isolados.
 * Retorna NULL em caso de erro. */
Grafo *kruskal_construir_floresta(const Grafo *grafo, int limiar_peso);

#endif
