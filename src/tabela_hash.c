#include "tabela_hash.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct EntradaHash {
    char *chave;
    int id;
    struct EntradaHash *proxima;
} EntradaHash;

struct TabelaHash {
    EntradaHash **baldes;
    size_t capacidade;
    size_t tamanho;
};

static uint64_t calcular_hash(const char *chave)
{
    uint64_t hash = UINT64_C(14695981039346656037);

    while (*chave != '\0') {
        hash ^= (unsigned char)*chave;
        hash *= UINT64_C(1099511628211);
        chave++;
    }

    return hash;
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

TabelaHash *tabela_hash_criar(size_t capacidade)
{
    TabelaHash *tabela;

    if (capacidade == 0) {
        return NULL;
    }

    tabela = malloc(sizeof(*tabela));
    if (tabela == NULL) {
        return NULL;
    }

    tabela->baldes = calloc(capacidade, sizeof(*tabela->baldes));
    if (tabela->baldes == NULL) {
        free(tabela);
        return NULL;
    }

    tabela->capacidade = capacidade;
    tabela->tamanho = 0;
    return tabela;
}

void tabela_hash_destruir(TabelaHash *tabela)
{
    size_t i;

    if (tabela == NULL) {
        return;
    }

    for (i = 0; i < tabela->capacidade; i++) {
        EntradaHash *entrada = tabela->baldes[i];

        while (entrada != NULL) {
            EntradaHash *proxima = entrada->proxima;
            free(entrada->chave);
            free(entrada);
            entrada = proxima;
        }
    }

    free(tabela->baldes);
    free(tabela);
}

int tabela_hash_inserir(TabelaHash *tabela, const char *chave, int id)
{
    size_t indice;
    EntradaHash *entrada;

    if (tabela == NULL || chave == NULL || chave[0] == '\0') {
        return 0;
    }

    indice = (size_t)(calcular_hash(chave) % tabela->capacidade);
    entrada = tabela->baldes[indice];

    while (entrada != NULL) {
        if (strcmp(entrada->chave, chave) == 0) {
            return 0;
        }
        entrada = entrada->proxima;
    }

    entrada = malloc(sizeof(*entrada));
    if (entrada == NULL) {
        return 0;
    }

    entrada->chave = copiar_string(chave);
    if (entrada->chave == NULL) {
        free(entrada);
        return 0;
    }

    entrada->id = id;
    entrada->proxima = tabela->baldes[indice];
    tabela->baldes[indice] = entrada;
    tabela->tamanho++;
    return 1;
}

int tabela_hash_buscar(const TabelaHash *tabela, const char *chave, int *id)
{
    size_t indice;
    EntradaHash *entrada;

    if (tabela == NULL || chave == NULL || id == NULL) {
        return 0;
    }

    indice = (size_t)(calcular_hash(chave) % tabela->capacidade);
    entrada = tabela->baldes[indice];

    while (entrada != NULL) {
        if (strcmp(entrada->chave, chave) == 0) {
            *id = entrada->id;
            return 1;
        }
        entrada = entrada->proxima;
    }

    return 0;
}

size_t tabela_hash_tamanho(const TabelaHash *tabela)
{
    return tabela == NULL ? 0 : tabela->tamanho;
}
