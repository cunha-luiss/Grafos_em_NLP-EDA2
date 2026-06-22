#include "tabela_hash.h"

#include <assert.h>
#include <stdio.h>

static void testar_criacao(void)
{
    TabelaHash *tabela = tabela_hash_criar(8);

    assert(tabela != NULL);
    assert(tabela_hash_tamanho(tabela) == 0);

    tabela_hash_destruir(tabela);
}

static void testar_insercao_e_busca(void)
{
    TabelaHash *tabela = tabela_hash_criar(8);
    int id = -1;

    assert(tabela_hash_inserir(tabela, "Atletismo", 0));
    assert(tabela_hash_inserir(tabela, "corrida", 1));
    assert(tabela_hash_buscar(tabela, "Atletismo", &id));
    assert(id == 0);
    assert(tabela_hash_buscar(tabela, "corrida", &id));
    assert(id == 1);
    assert(tabela_hash_tamanho(tabela) == 2);

    tabela_hash_destruir(tabela);
}

static void testar_chaves_duplicadas(void)
{
    TabelaHash *tabela = tabela_hash_criar(8);
    int id = -1;

    assert(tabela_hash_inserir(tabela, "futebol", 10));
    assert(!tabela_hash_inserir(tabela, "futebol", 20));
    assert(tabela_hash_buscar(tabela, "futebol", &id));
    assert(id == 10);
    assert(tabela_hash_tamanho(tabela) == 1);

    tabela_hash_destruir(tabela);
}

static void testar_colisoes(void)
{
    TabelaHash *tabela = tabela_hash_criar(1);
    int id = -1;

    assert(tabela_hash_inserir(tabela, "bola", 1));
    assert(tabela_hash_inserir(tabela, "quadra", 2));
    assert(tabela_hash_inserir(tabela, "equipe", 3));

    assert(tabela_hash_buscar(tabela, "bola", &id) && id == 1);
    assert(tabela_hash_buscar(tabela, "quadra", &id) && id == 2);
    assert(tabela_hash_buscar(tabela, "equipe", &id) && id == 3);

    tabela_hash_destruir(tabela);
}

static void testar_entradas_invalidas(void)
{
    TabelaHash *tabela = tabela_hash_criar(8);
    int id = -1;

    assert(tabela_hash_criar(0) == NULL);
    assert(!tabela_hash_inserir(NULL, "bola", 1));
    assert(!tabela_hash_inserir(tabela, NULL, 1));
    assert(!tabela_hash_inserir(tabela, "", 1));
    assert(!tabela_hash_buscar(tabela, "inexistente", &id));
    assert(!tabela_hash_buscar(tabela, "bola", NULL));

    tabela_hash_destruir(tabela);
    tabela_hash_destruir(NULL);
}

int main(void)
{
    testar_criacao();
    testar_insercao_e_busca();
    testar_chaves_duplicadas();
    testar_colisoes();
    testar_entradas_invalidas();

    puts("Todos os testes da tabela hash passaram.");
    return 0;
}
