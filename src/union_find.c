#include "union_find.h"

#include <stdlib.h>

struct UnionFind
{
    size_t quantidade;
    size_t *pai;
    size_t *tamanho;
};

static int indice_valido(const UnionFind *union_find, int elemento)
{
    return union_find != NULL && elemento >= 0 && (size_t)elemento < union_find->quantidade;
}

static size_t encontrar_raiz(UnionFind *union_find, size_t elemento)
{
    if (union_find->pai[elemento] != elemento)
    {
        union_find->pai[elemento] = encontrar_raiz(
            union_find,
            union_find->pai[elemento]);
    }

    return union_find->pai[elemento];
}

UnionFind *union_find_criar(size_t quantidade)
{
    UnionFind *union_find;
    size_t i;

    if (quantidade == 0)
    {
        return NULL;
    }

    union_find = malloc(sizeof(*union_find));
    if (union_find == NULL)
    {
        return NULL;
    }

    union_find->pai = malloc(quantidade * sizeof(*union_find->pai));
    if (union_find->pai == NULL)
    {
        free(union_find);
        return NULL;
    }

    union_find->tamanho = malloc(quantidade * sizeof(*union_find->tamanho));
    if (union_find->tamanho == NULL)
    {
        free(union_find->pai);
        free(union_find);
        return NULL;
    }

    union_find->quantidade = quantidade;

    for (i = 0; i < quantidade; i++)
    {
        union_find->pai[i] = i;
        union_find->tamanho[i] = 1;
    }

    return union_find;
}

void union_find_destruir(UnionFind *union_find)
{
    if (union_find == NULL)
    {
        return;
    }

    free(union_find->tamanho);
    free(union_find->pai);
    free(union_find);
}

int union_find_raiz(UnionFind *union_find, int elemento)
{
    if (!indice_valido(union_find, elemento))
    {
        return -1;
    }

    return (int)encontrar_raiz(union_find, (size_t)elemento);
}

int union_find_unir(UnionFind *union_find, int a, int b)
{
    size_t raiz_a;
    size_t raiz_b;

    if (!indice_valido(union_find, a) || !indice_valido(union_find, b))
    {
        return 0;
    }

    raiz_a = encontrar_raiz(union_find, (size_t)a);
    raiz_b = encontrar_raiz(union_find, (size_t)b);

    if (raiz_a == raiz_b)
    {
        return 0;
    }

    if (union_find->tamanho[raiz_a] < union_find->tamanho[raiz_b])
    {
        size_t temp = raiz_a;
        raiz_a = raiz_b;
        raiz_b = temp;
    }

    union_find->pai[raiz_b] = raiz_a;
    union_find->tamanho[raiz_a] += union_find->tamanho[raiz_b];
    return 1;
}