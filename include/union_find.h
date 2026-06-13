#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <stddef.h>

typedef struct UnionFind UnionFind;

UnionFind *union_find_criar(size_t quantidade);

void union_find_destruir(UnionFind *union_find);

int union_find_raiz(UnionFind *union_find, int elemento);

int union_find_unir(UnionFind *union_find, int a, int b);

#endif