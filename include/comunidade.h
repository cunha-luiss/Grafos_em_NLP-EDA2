#ifndef COMUNIDADE_H
#define COMUNIDADE_H

#include "grafo.h"
#include "catalogo.h"
#include <stddef.h>

typedef struct {
    int    *vertices;      
    size_t  tamanho;       
} Comunidade;

typedef struct {
    Comunidade *itens;
    size_t      quantidade;
} ListaComunidades;

ListaComunidades *comunidade_detectar(const Grafo *floresta);


void comunidade_imprimir(const ListaComunidades *lista,
                         const Catalogo *catalogo);

void comunidade_imprimir_com_caracteristicas(
    const ListaComunidades *lista,
    const Catalogo *catalogo,
    const DadosIdentificados *dados);

void comunidade_destruir(ListaComunidades *lista);

#endif
