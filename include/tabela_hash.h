#ifndef TABELA_HASH_H
#define TABELA_HASH_H

#include <stddef.h>

typedef struct TabelaHash TabelaHash;

TabelaHash *tabela_hash_criar(size_t capacidade);
void tabela_hash_destruir(TabelaHash *tabela);

int tabela_hash_inserir(TabelaHash *tabela, const char *chave, int id);
int tabela_hash_buscar(const TabelaHash *tabela, const char *chave, int *id);

size_t tabela_hash_tamanho(const TabelaHash *tabela);

#endif
