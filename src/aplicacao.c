#include "aplicacao.h"

#include "catalogo.h"
#include "dados_esportes.h"
#include "dados_identificados.h"
#include "leitor_arquivo.h"
#include "grafo.h"
#include "heap.h"

#include <stdlib.h>

int aplicacao_executar(
    const char *caminho_json,
    FILE *saida,
    FILE *saida_erros
)
{
    char *json;
    size_t tamanho_json;
    size_t quantidade_associacoes = 0;
    size_t quantidade_vertices;
    size_t quantidade_esportes_unicos = 0;
    size_t quantidade_caracteristicas = 0;
    size_t i;
    DadosEsportes dados_esportes = {NULL, 0};
    DadosIdentificados dados_identificados = {NULL, NULL, 0};

    if (
        caminho_json == NULL ||
        saida == NULL ||
        saida_erros == NULL
    ) {
        return 1;
    }

    json = ler_arquivo_texto(caminho_json, &tamanho_json);
    if (json == NULL) {
        fprintf(
            saida_erros,
            "Erro: nao foi possivel ler o arquivo '%s'.\n",
            caminho_json
        );
        return 1;
    }

    if (!dados_esportes_ler_json(
        json,
        tamanho_json,
        &dados_esportes
    )) {
        fprintf(
            saida_erros,
            "Erro: o arquivo '%s' nao possui o formato esperado.\n",
            caminho_json
        );
        free(json);
        return 1;
    }

    if (!dados_identificados_criar(
        &dados_esportes,
        &dados_identificados
    )) {
        fputs(
            "Erro: nao foi possivel gerar os IDs dos vertices.\n",
            saida_erros
        );
        dados_esportes_destruir(&dados_esportes);
        free(json);
        return 1;
    }

    for (i = 0; i < dados_identificados.quantidade_esportes; i++) {
        quantidade_associacoes +=
            dados_identificados.esportes[i].quantidade_caracteristicas;
    }

    quantidade_vertices = catalogo_tamanho(
        dados_identificados.catalogo
    );

    for (i = 0; i < quantidade_vertices; i++) {
        TipoVertice tipo;

        if (!catalogo_obter_tipo(
            dados_identificados.catalogo,
            (int)i,
            &tipo
        )) {
            fputs(
                "Erro: catalogo de IDs inconsistente.\n",
                saida_erros
            );
            dados_identificados_destruir(&dados_identificados);
            dados_esportes_destruir(&dados_esportes);
            free(json);
            return 1;
        }

        if (tipo == TIPO_ESPORTE) {
            quantidade_esportes_unicos++;
        } else {
            quantidade_caracteristicas++;
        }
    }

    fprintf(saida, "Arquivo: %s\n", caminho_json);
    fprintf(
        saida,
        "Esportes: %zu\n",
        quantidade_esportes_unicos
    );
    fprintf(
        saida,
        "Caracteristicas unicas: %zu\n",
        quantidade_caracteristicas
    );
    fprintf(saida, "Vertices totais: %zu\n", quantidade_vertices);
    fprintf(
        saida,
        "Associacoes esporte-caracteristica: %zu\n",
        quantidade_associacoes
    );

    Grafo *grafo = grafo_construir_de_dados(&dados_identificados, quantidade_vertices);
    if (grafo == NULL) {
        fputs("Erro: nao foi possivel construir o grafo.\n", saida_erros);
        dados_identificados_destruir(&dados_identificados);
        dados_esportes_destruir(&dados_esportes);
        free(json);
        return 1;
    }
    
    fprintf(saida, "Grafo construido com sucesso!\n");

    {
        HeapAresta *heap = heap_construir_de_grafo(grafo);
        const size_t quantidade_exibida = 5;
        size_t i;

        if (heap == NULL) {
            fputs(
                "Erro: nao foi possivel construir a heap de arestas.\n",
                saida_erros
            );
            grafo_destruir(grafo);
            dados_identificados_destruir(&dados_identificados);
            dados_esportes_destruir(&dados_esportes);
            free(json);
            return 1;
        }

        fputs("Associacoes mais frequentes na Wikipedia:\n", saida);

        for (i = 0; i < quantidade_exibida; i++) {
            ItemHeap item;
            int id_esporte;
            int id_caracteristica;
            TipoVertice tipo_origem;

            if (!heap_remover_maximo(heap, &item)) {
                break;
            }

            if (!catalogo_obter_tipo(
                    dados_identificados.catalogo,
                    item.origem,
                    &tipo_origem
                )) {
                continue;
            }

            if (tipo_origem == TIPO_ESPORTE) {
                id_esporte = item.origem;
                id_caracteristica = item.destino;
            } else {
                id_esporte = item.destino;
                id_caracteristica = item.origem;
            }

            fprintf(
                saida,
                "  %s - %s: %d\n",
                catalogo_obter_texto(dados_identificados.catalogo, id_esporte),
                catalogo_obter_texto(dados_identificados.catalogo, id_caracteristica),
                item.peso
            );
        }

        heap_destruir(heap);
    }

    grafo_destruir(grafo);
    dados_identificados_destruir(&dados_identificados);
    dados_esportes_destruir(&dados_esportes);
    free(json);
    return 0;
}
