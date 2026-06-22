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

static const EsporteIdentificado *buscar_esporte_por_id(
    const DadosIdentificados *dados,
    int id_esporte)
{
    size_t i;

    if (dados == NULL)
    {
        return NULL;
    }

    for (i = 0; i < dados->quantidade_esportes; i++)
    {
        if (dados->esportes[i].id_esporte == id_esporte)
        {
            return &dados->esportes[i];
        }
    }

    return NULL;
}

static int caracteristica_repetida_antes(
    const EsporteIdentificado *esporte,
    size_t posicao)
{
    size_t i;
    int id;

    if (esporte == NULL || posicao >= esporte->quantidade_caracteristicas)
    {
        return 0;
    }

    id = esporte->ids_caracteristicas[posicao];
    for (i = 0; i < posicao; i++)
    {
        if (esporte->ids_caracteristicas[i] == id)
        {
            return 1;
        }
    }

    return 0;
}

static void imprimir_caracteristicas_compartilhadas(
    const Comunidade *com,
    const Catalogo *catalogo,
    const DadosIdentificados *dados)
{
    size_t quantidade_vertices = catalogo_tamanho(catalogo);
    int *contagens = calloc(quantidade_vertices, sizeof(*contagens));
    size_t i, j;

    if (contagens == NULL)
    {
        return;
    }

    for (i = 0; i < com->tamanho; i++)
    {
        int id_vertice = com->vertices[i];
        TipoVertice tipo;
        const EsporteIdentificado *esporte;

        if (!catalogo_obter_tipo(catalogo, id_vertice, &tipo))
        {
            continue;
        }

        if (tipo != TIPO_ESPORTE)
        {
            continue;
        }

        esporte = buscar_esporte_por_id(dados, id_vertice);
        if (esporte == NULL)
        {
            continue;
        }

        for (j = 0; j < esporte->quantidade_caracteristicas; j++)
        {
            int id_caracteristica = esporte->ids_caracteristicas[j];

            if (
                id_caracteristica >= 0 &&
                (size_t)id_caracteristica < quantidade_vertices &&
                !caracteristica_repetida_antes(esporte, j))
            {
                contagens[id_caracteristica]++;
            }
        }
    }

    for (i = 0; i < quantidade_vertices; i++)
    {
        if (contagens[i] >= 2)
        {
            printf("  - %s\n", catalogo_obter_texto(catalogo, (int)i));
        }
    }

    free(contagens);
}

static size_t contar_esportes(
    const Comunidade *com,
    const Catalogo *catalogo)
{
    size_t i;
    size_t quantidade = 0;

    for (i = 0; i < com->tamanho; i++)
    {
        TipoVertice tipo;

        if (
            catalogo_obter_tipo(catalogo, com->vertices[i], &tipo) &&
            tipo == TIPO_ESPORTE)
        {
            quantidade++;
        }
    }

    return quantidade;
}

static void imprimir_comunidades(
    const ListaComunidades *lista,
    const Catalogo *catalogo,
    const DadosIdentificados *dados)
{
    size_t i, j;
    size_t comunidades_com_grupo = 0;
    size_t esportes_isolados = 0;
    size_t primeiro_isolado = 1;

    if (lista == NULL)
    {
        return;
    }

    /* Conta apenas comunidades com pelo menos dois esportes. */
    for (i = 0; i < lista->quantidade; i++)
    {
        size_t quantidade_esportes = contar_esportes(&lista->itens[i], catalogo);

        if (quantidade_esportes >= 2)
        {
            comunidades_com_grupo++;
        }
        else if (quantidade_esportes == 1)
        {
            esportes_isolados++;
        }
    }

    printf("Comunidades detectadas: %zu\n\n", comunidades_com_grupo);

    for (i = 0; i < lista->quantidade; i++)
    {
        const Comunidade *com = &lista->itens[i];
        size_t quantidade_esportes = contar_esportes(com, catalogo);
        size_t primeiro_esporte = 1;

        if (quantidade_esportes < 2)
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
        if (dados != NULL)
        {
            imprimir_caracteristicas_compartilhadas(com, catalogo, dados);
        }
        else
        {
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
        }
        printf("\n");
    }

    if (esportes_isolados > 0)
    {
        printf("Esportes sem comunidade (%zu): ", esportes_isolados);

        for (i = 0; i < lista->quantidade; i++)
        {
            const Comunidade *com = &lista->itens[i];

            if (contar_esportes(com, catalogo) != 1)
            {
                continue;
            }

            for (j = 0; j < com->tamanho; j++)
            {
                int id = com->vertices[j];
                TipoVertice tipo;

                if (
                    catalogo_obter_tipo(catalogo, id, &tipo) &&
                    tipo == TIPO_ESPORTE)
                {
                    if (!primeiro_isolado)
                    {
                        printf(", ");
                    }

                    printf("%s", catalogo_obter_texto(catalogo, id));
                    primeiro_isolado = 0;
                    break;
                }
            }
        }

        printf("\n\n");
    }
}

void comunidade_imprimir(const ListaComunidades *lista,
                          const Catalogo *catalogo)
{
    imprimir_comunidades(lista, catalogo, NULL);
}

void comunidade_imprimir_com_caracteristicas(
    const ListaComunidades *lista,
    const Catalogo *catalogo,
    const DadosIdentificados *dados)
{
    imprimir_comunidades(lista, catalogo, dados);
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
