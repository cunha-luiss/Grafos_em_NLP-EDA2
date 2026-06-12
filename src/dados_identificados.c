#include "dados_identificados.h"

#include <stdint.h>
#include <stdlib.h>

static void inicializar_vazio(DadosIdentificados *dados)
{
    dados->catalogo = NULL;
    dados->esportes = NULL;
    dados->quantidade_esportes = 0;
}

void dados_identificados_destruir(DadosIdentificados *dados)
{
    size_t i;

    if (dados == NULL) {
        return;
    }

    for (i = 0; i < dados->quantidade_esportes; i++) {
        free(dados->esportes[i].ids_caracteristicas);
    }

    free(dados->esportes);
    catalogo_destruir(dados->catalogo);
    inicializar_vazio(dados);
}

static int identificar_esporte(
    Catalogo *catalogo,
    const RegistroEsporte *origem,
    EsporteIdentificado *destino
)
{
    size_t i;

    destino->id_esporte = -1;
    destino->ids_caracteristicas = NULL;
    destino->quantidade_caracteristicas = 0;

    if (!catalogo_obter_ou_inserir(
        catalogo,
        origem->nome,
        TIPO_ESPORTE,
        &destino->id_esporte
    )) {
        return 0;
    }

    if (origem->quantidade_caracteristicas == 0) {
        return 1;
    }

    if (
        origem->quantidade_caracteristicas >
        SIZE_MAX / sizeof(*destino->ids_caracteristicas)
    ) {
        return 0;
    }

    destino->ids_caracteristicas = malloc(
        origem->quantidade_caracteristicas *
        sizeof(*destino->ids_caracteristicas)
    );
    if (destino->ids_caracteristicas == NULL) {
        return 0;
    }

    for (i = 0; i < origem->quantidade_caracteristicas; i++) {
        if (!catalogo_obter_ou_inserir(
            catalogo,
            origem->caracteristicas[i],
            TIPO_CARACTERISTICA,
            &destino->ids_caracteristicas[i]
        )) {
            return 0;
        }

        destino->quantidade_caracteristicas++;
    }

    return 1;
}

int dados_identificados_criar(
    const DadosEsportes *origem,
    DadosIdentificados *destino
)
{
    DadosIdentificados resultado;
    size_t i;

    if (origem == NULL || destino == NULL) {
        return 0;
    }

    inicializar_vazio(destino);
    inicializar_vazio(&resultado);

    resultado.catalogo = catalogo_criar(1024);
    if (resultado.catalogo == NULL) {
        return 0;
    }

    if (origem->quantidade > 0) {
        if (
            origem->registros == NULL ||
            origem->quantidade >
            SIZE_MAX / sizeof(*resultado.esportes)
        ) {
            dados_identificados_destruir(&resultado);
            return 0;
        }

        resultado.esportes = calloc(
            origem->quantidade,
            sizeof(*resultado.esportes)
        );
        if (resultado.esportes == NULL) {
            dados_identificados_destruir(&resultado);
            return 0;
        }
    }

    for (i = 0; i < origem->quantidade; i++) {
        if (!identificar_esporte(
            resultado.catalogo,
            &origem->registros[i],
            &resultado.esportes[i]
        )) {
            resultado.quantidade_esportes = i + 1;
            dados_identificados_destruir(&resultado);
            return 0;
        }

        resultado.quantidade_esportes++;
    }

    *destino = resultado;
    return 1;
}
