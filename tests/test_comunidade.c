#include "comunidade.h"
#include "grafo.h"

#include <assert.h>
#include <stdio.h>

/* Retorna 1 se a comunidade contiver o vertice, 0 caso contrario. */
static int comunidade_contem(const Comunidade *com, int vertice)
{
    size_t i;
    for (i = 0; i < com->tamanho; i++)
    {
        if (com->vertices[i] == vertice)
        {
            return 1;
        }
    }
    return 0;
}

/* Retorna o indice da comunidade que contem o vertice, ou -1 se nao encontrar. */
static int indice_comunidade_do_vertice(const ListaComunidades *lista, int vertice)
{
    size_t i;
    for (i = 0; i < lista->quantidade; i++)
    {
        if (comunidade_contem(&lista->itens[i], vertice))
        {
            return (int)i;
        }
    }
    return -1;
}

/* Grafo conexo: 0-1-2 em linha -> 1 comunidade com 3 membros. */
static void test_grafo_conexo(void)
{
    Grafo *floresta;
    ListaComunidades *lista;

    floresta = grafo_criar(3);
    assert(floresta != NULL);
    assert(grafo_adicionar_aresta(floresta, 0, 1));
    assert(grafo_adicionar_aresta(floresta, 1, 2));

    lista = comunidade_detectar(floresta);
    assert(lista != NULL);
    assert(lista->quantidade == 1);
    assert(lista->itens[0].tamanho == 3);
    assert(comunidade_contem(&lista->itens[0], 0));
    assert(comunidade_contem(&lista->itens[0], 1));
    assert(comunidade_contem(&lista->itens[0], 2));

    comunidade_destruir(lista);
    grafo_destruir(floresta);
    printf("test_grafo_conexo: OK\n");
}

/* Grafo com 2 componentes: {0,1} e {2,3} -> 2 comunidades. */
static void test_dois_componentes(void)
{
    Grafo *floresta;
    ListaComunidades *lista;
    int idx0, idx2;

    floresta = grafo_criar(4);
    assert(floresta != NULL);
    assert(grafo_adicionar_aresta(floresta, 0, 1));
    assert(grafo_adicionar_aresta(floresta, 2, 3));

    lista = comunidade_detectar(floresta);
    assert(lista != NULL);
    assert(lista->quantidade == 2);

    /* 0 e 1 devem estar na mesma comunidade */
    idx0 = indice_comunidade_do_vertice(lista, 0);
    assert(idx0 != -1);
    assert(comunidade_contem(&lista->itens[idx0], 1));

    /* 2 e 3 devem estar na mesma comunidade, diferente de {0,1} */
    idx2 = indice_comunidade_do_vertice(lista, 2);
    assert(idx2 != -1);
    assert(comunidade_contem(&lista->itens[idx2], 3));
    assert(idx0 != idx2);

    comunidade_destruir(lista);
    grafo_destruir(floresta);
    printf("test_dois_componentes: OK\n");
}

/* Grafo sem arestas: 3 vertices isolados -> 3 comunidades de tamanho 1. */
static void test_vertices_isolados(void)
{
    Grafo *floresta;
    ListaComunidades *lista;
    size_t i;

    floresta = grafo_criar(3);
    assert(floresta != NULL);

    lista = comunidade_detectar(floresta);
    assert(lista != NULL);
    assert(lista->quantidade == 3);
    for (i = 0; i < lista->quantidade; i++)
    {
        assert(lista->itens[i].tamanho == 1);
    }

    comunidade_destruir(lista);
    grafo_destruir(floresta);
    printf("test_vertices_isolados: OK\n");
}

/* Entrada NULL -> retorna NULL sem crash. */
static void test_entrada_nula(void)
{
    ListaComunidades *lista = comunidade_detectar(NULL);
    assert(lista == NULL);
    printf("test_entrada_nula: OK\n");
}

/* comunidade_destruir(NULL) nao deve crashar. */
static void test_destruir_nulo(void)
{
    comunidade_destruir(NULL);
    printf("test_destruir_nulo: OK\n");
}

int main(void)
{
    test_grafo_conexo();
    test_dois_componentes();
    test_vertices_isolados();
    test_entrada_nula();
    test_destruir_nulo();

    printf("\nTodos os testes de comunidade passaram.\n");
    return 0;
}
