#ifndef DADOS_IDENTIFICADOS_H
#define DADOS_IDENTIFICADOS_H

#include "catalogo.h"
#include "dados_esportes.h"

#include <stddef.h>

typedef struct {
    int id_esporte;
    int *ids_caracteristicas;
    size_t quantidade_caracteristicas;
} EsporteIdentificado;

typedef struct {
    Catalogo *catalogo;
    EsporteIdentificado *esportes;
    size_t quantidade_esportes;
} DadosIdentificados;

int dados_identificados_criar(
    const DadosEsportes *origem,
    DadosIdentificados *destino
);

void dados_identificados_destruir(DadosIdentificados *dados);

#endif
