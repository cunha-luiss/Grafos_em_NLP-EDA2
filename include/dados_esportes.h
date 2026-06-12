#ifndef DADOS_ESPORTES_H
#define DADOS_ESPORTES_H

#include <stddef.h>

typedef struct {
    char *nome;
    char **caracteristicas;
    size_t quantidade_caracteristicas;
} RegistroEsporte;

typedef struct {
    RegistroEsporte *registros;
    size_t quantidade;
} DadosEsportes;

int dados_esportes_ler_json(
    const char *texto,
    size_t tamanho,
    DadosEsportes *dados
);

void dados_esportes_destruir(DadosEsportes *dados);

#endif
