#include "union_find.h"

#include <assert.h>
#include <stdio.h>

static void testar_criacao_e_raizes_iniciais(void)
{
    UnionFind *union_find = union_find_criar(5);
    size_t i;

    assert(union_find != NULL);

    for (i = 0; i < 5; i++)
    {
        assert(union_find_raiz(union_find, (int)i) == (int)i);
    }

    assert(union_find_criar(0) == NULL);
    assert(union_find_raiz(NULL, 0) == -1);

    union_find_destruir(union_find);
    union_find_destruir(NULL);
}

static void testar_uniao_e_compressao(void)
{
    UnionFind *union_find = union_find_criar(6);

    assert(union_find != NULL);

    assert(union_find_unir(union_find, 0, 1));
    assert(union_find_unir(union_find, 2, 3));
    assert(union_find_unir(union_find, 4, 5));
    assert(union_find_unir(union_find, 1, 2));

    assert(union_find_raiz(union_find, 0) == union_find_raiz(union_find, 3));
    assert(union_find_raiz(union_find, 4) == union_find_raiz(union_find, 5));
    assert(!union_find_unir(union_find, 0, 3));

    union_find_destruir(union_find);
}

static void testar_entradas_invalidas(void)
{
    assert(!union_find_unir(NULL, 0, 1));
    assert(union_find_raiz(NULL, 1) == -1);
}

int main(void)
{
    testar_criacao_e_raizes_iniciais();
    testar_uniao_e_compressao();
    testar_entradas_invalidas();

    puts("Todos os testes do union-find passaram.");
    return 0;
}