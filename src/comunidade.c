#include "comunidade.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* BFS com fila simples */

static int bfs(
    const Grafo *floresta,
    int raiz,
    int *visitado,
    int *componente,
    size_t *tamanho_componente)
{
    
    int *fila = malloc(floresta->quantidade_vertices * sizeof(int));
    if (fila == NULL)
    {
        return 0;
    }

    size_t cabeca = 0;
    size_t cauda  = 0;
    size_t tam    = 0;

    visitado[raiz]        = 1;
    fila[cauda++]         = raiz;
    componente[tam++]     = raiz;

    while (cabeca < cauda)
    {
        int atual      = fila[cabeca++];
        Aresta *aresta = floresta->lista_adj[atual];

        while (aresta != NULL)
        {
            if (!visitado[aresta->vizinho])
            {
                visitado[aresta->vizinho]  = 1;
                fila[cauda++]              = aresta->vizinho;
                componente[tam++]          = aresta->vizinho;
            }
            aresta = aresta->proxima;
        }
    }

    free(fila);
    *tamanho_componente = tam;
    return 1;
}

/* API publica */

ListaComunidades *comunidade_detectar(const Grafo *floresta)
{
    if (floresta == NULL || floresta->quantidade_vertices == 0)
    {
        return NULL;
    }

    size_t n = floresta->quantidade_vertices;

    int *visitado = calloc(n, sizeof(int));
    if (visitado == NULL)
    {
        return NULL;
    }

    int *componente_buf = malloc(n * sizeof(int));
    if (componente_buf == NULL)
    {
        free(visitado);
        return NULL;
    }

    ListaComunidades *lista = malloc(sizeof(ListaComunidades));
    if (lista == NULL)
    {
        free(visitado);
        free(componente_buf);
        return NULL;
    }
    lista->itens      = NULL;
    lista->quantidade = 0;

    for (size_t i = 0; i < n; i++)
    {
        if (visitado[i])
        {
            continue;
        }

        size_t tamanho = 0;
        if (!bfs(floresta, (int)i, visitado, componente_buf, &tamanho))
        {
            comunidade_destruir(lista);
            free(visitado);
            free(componente_buf);
            return NULL;
        }

        /* adiciona nova comunidade a lista */
        Comunidade *novo_itens = realloc(
            lista->itens,
            (lista->quantidade + 1) * sizeof(Comunidade));
        if (novo_itens == NULL)
        {
            comunidade_destruir(lista);
            free(visitado);
            free(componente_buf);
            return NULL;
        }
        lista->itens = novo_itens;

        Comunidade *com = &lista->itens[lista->quantidade];
        com->vertices   = malloc(tamanho * sizeof(int));
        if (com->vertices == NULL)
        {
            comunidade_destruir(lista);
            free(visitado);
            free(componente_buf);
            return NULL;
        }
        memcpy(com->vertices, componente_buf, tamanho * sizeof(int));
        com->tamanho = tamanho;
        lista->quantidade++;
    }

    free(visitado);
    free(componente_buf);
    return lista;
}

void comunidade_imprimir(const ListaComunidades *lista,
                          const Catalogo *catalogo)
{
    size_t i, j;
    size_t comunidades_com_esporte = 0;

    if (lista == NULL)
    {
        return;
    }

    /* conta apenas comunidades que possuem ao menos um esporte */
    for (i = 0; i < lista->quantidade; i++)
    {
        const Comunidade *com = &lista->itens[i];
        for (j = 0; j < com->tamanho; j++)
        {
            TipoVertice tipo;
            catalogo_obter_tipo(catalogo, com->vertices[j], &tipo);
            if (tipo == TIPO_ESPORTE)
            {
                comunidades_com_esporte++;
                break;
            }
        }
    }

    printf("Comunidades detectadas: %zu\n\n", comunidades_com_esporte);

    for (i = 0; i < lista->quantidade; i++)
    {
        const Comunidade *com = &lista->itens[i];
        int tem_esporte = 0;
        size_t primeiro_esporte = 1;

        /* verifica se ha esporte nesta comunidade */
        for (j = 0; j < com->tamanho; j++)
        {
            TipoVertice tipo;
            catalogo_obter_tipo(catalogo, com->vertices[j], &tipo);
            if (tipo == TIPO_ESPORTE)
            {
                tem_esporte = 1;
                break;
            }
        }

        if (!tem_esporte)
        {
            continue;
        }

        /* imprime header */
        printf("Comunidade: ");
        primeiro_esporte = 1;
        for (j = 0; j < com->tamanho; j++)
        {
            int id = com->vertices[j];
            TipoVertice tipo;
            catalogo_obter_tipo(catalogo, id, &tipo);
            if (tipo == TIPO_ESPORTE)
            {
                if (!primeiro_esporte)
                {
                    printf(", ");
                }
                printf("%s", catalogo_obter_texto(catalogo, id));
                primeiro_esporte = 0;
            }
        }
        printf("\n");

        /* imprime caracteristicas abaixo */
        for (j = 0; j < com->tamanho; j++)
        {
            int id = com->vertices[j];
            TipoVertice tipo;
            catalogo_obter_tipo(catalogo, id, &tipo);
            if (tipo == TIPO_CARACTERISTICA)
            {
                printf("  - %s\n", catalogo_obter_texto(catalogo, id));
            }
        }
        printf("\n");
    }
}

void comunidade_destruir(ListaComunidades *lista)
{
    if (lista == NULL)
    {
        return;
    }

    for (size_t i = 0; i < lista->quantidade; i++)
    {
        free(lista->itens[i].vertices);
    }
    free(lista->itens);
    free(lista);
}
