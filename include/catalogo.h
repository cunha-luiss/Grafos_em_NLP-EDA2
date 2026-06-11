#ifndef CATALOGO_H
#define CATALOGO_H

#include <stddef.h>

typedef enum {
    TIPO_ESPORTE = 0,
    TIPO_CARACTERISTICA = 1
} TipoVertice;

typedef struct Catalogo Catalogo;

Catalogo *catalogo_criar(size_t capacidade_hash);
void catalogo_destruir(Catalogo *catalogo);

int catalogo_obter_ou_inserir(
    Catalogo *catalogo,
    const char *texto,
    TipoVertice tipo,
    int *id
);

int catalogo_buscar(
    const Catalogo *catalogo,
    const char *texto,
    TipoVertice tipo,
    int *id
);

const char *catalogo_obter_texto(const Catalogo *catalogo, int id);
int catalogo_obter_tipo(
    const Catalogo *catalogo,
    int id,
    TipoVertice *tipo
);

size_t catalogo_tamanho(const Catalogo *catalogo);

#endif
