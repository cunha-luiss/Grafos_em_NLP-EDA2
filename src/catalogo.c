#include "catalogo.h"

#include "tabela_hash.h"

#include <stdlib.h>
#include <string.h>

typedef struct {
    char *texto;
    TipoVertice tipo;
} VerticeCatalogo;

struct Catalogo {
    TabelaHash *indice;
    VerticeCatalogo *vertices;
    size_t tamanho;
    size_t capacidade;
};

static int tipo_valido(TipoVertice tipo)
{
    return tipo == TIPO_ESPORTE || tipo == TIPO_CARACTERISTICA;
}

static char *copiar_string(const char *origem)
{
    size_t tamanho = strlen(origem) + 1;
    char *copia = malloc(tamanho);

    if (copia != NULL) {
        memcpy(copia, origem, tamanho);
    }

    return copia;
}

static char *criar_chave(const char *texto, TipoVertice tipo)
{
    size_t tamanho = strlen(texto) + 3;
    char *chave = malloc(tamanho);

    if (chave == NULL) {
        return NULL;
    }

    chave[0] = tipo == TIPO_ESPORTE ? 'E' : 'C';
    chave[1] = ':';
    memcpy(chave + 2, texto, tamanho - 2);
    return chave;
}

static int expandir_vertices(Catalogo *catalogo)
{
    size_t nova_capacidade;
    VerticeCatalogo *novos_vertices;

    if (catalogo->tamanho < catalogo->capacidade) {
        return 1;
    }

    nova_capacidade = catalogo->capacidade == 0
        ? 8
        : catalogo->capacidade * 2;

    novos_vertices = realloc(
        catalogo->vertices,
        nova_capacidade * sizeof(*novos_vertices)
    );
    if (novos_vertices == NULL) {
        return 0;
    }

    catalogo->vertices = novos_vertices;
    catalogo->capacidade = nova_capacidade;
    return 1;
}

Catalogo *catalogo_criar(size_t capacidade_hash)
{
    Catalogo *catalogo;

    if (capacidade_hash == 0) {
        return NULL;
    }

    catalogo = malloc(sizeof(*catalogo));
    if (catalogo == NULL) {
        return NULL;
    }

    catalogo->indice = tabela_hash_criar(capacidade_hash);
    if (catalogo->indice == NULL) {
        free(catalogo);
        return NULL;
    }

    catalogo->vertices = NULL;
    catalogo->tamanho = 0;
    catalogo->capacidade = 0;
    return catalogo;
}

void catalogo_destruir(Catalogo *catalogo)
{
    size_t i;

    if (catalogo == NULL) {
        return;
    }

    for (i = 0; i < catalogo->tamanho; i++) {
        free(catalogo->vertices[i].texto);
    }

    free(catalogo->vertices);
    tabela_hash_destruir(catalogo->indice);
    free(catalogo);
}

int catalogo_buscar(
    const Catalogo *catalogo,
    const char *texto,
    TipoVertice tipo,
    int *id
)
{
    char *chave;
    int encontrado;

    if (
        catalogo == NULL ||
        texto == NULL ||
        texto[0] == '\0' ||
        !tipo_valido(tipo) ||
        id == NULL
    ) {
        return 0;
    }

    chave = criar_chave(texto, tipo);
    if (chave == NULL) {
        return 0;
    }

    encontrado = tabela_hash_buscar(catalogo->indice, chave, id);
    free(chave);
    return encontrado;
}

int catalogo_obter_ou_inserir(
    Catalogo *catalogo,
    const char *texto,
    TipoVertice tipo,
    int *id
)
{
    char *chave;
    char *copia;
    int novo_id;

    if (
        catalogo == NULL ||
        texto == NULL ||
        texto[0] == '\0' ||
        !tipo_valido(tipo) ||
        id == NULL
    ) {
        return 0;
    }

    if (catalogo_buscar(catalogo, texto, tipo, id)) {
        return 1;
    }

    if (!expandir_vertices(catalogo)) {
        return 0;
    }

    chave = criar_chave(texto, tipo);
    copia = copiar_string(texto);
    if (chave == NULL || copia == NULL) {
        free(chave);
        free(copia);
        return 0;
    }

    novo_id = (int)catalogo->tamanho;
    if (!tabela_hash_inserir(catalogo->indice, chave, novo_id)) {
        free(chave);
        free(copia);
        return 0;
    }

    catalogo->vertices[catalogo->tamanho].texto = copia;
    catalogo->vertices[catalogo->tamanho].tipo = tipo;
    catalogo->tamanho++;
    *id = novo_id;

    free(chave);
    return 1;
}

const char *catalogo_obter_texto(const Catalogo *catalogo, int id)
{
    if (
        catalogo == NULL ||
        id < 0 ||
        (size_t)id >= catalogo->tamanho
    ) {
        return NULL;
    }

    return catalogo->vertices[id].texto;
}

int catalogo_obter_tipo(
    const Catalogo *catalogo,
    int id,
    TipoVertice *tipo
)
{
    if (
        catalogo == NULL ||
        id < 0 ||
        (size_t)id >= catalogo->tamanho ||
        tipo == NULL
    ) {
        return 0;
    }

    *tipo = catalogo->vertices[id].tipo;
    return 1;
}

size_t catalogo_tamanho(const Catalogo *catalogo)
{
    return catalogo == NULL ? 0 : catalogo->tamanho;
}
