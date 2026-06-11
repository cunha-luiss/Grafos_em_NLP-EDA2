#include "dados_esportes.h"

#include "parser_json.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void registro_esporte_destruir(RegistroEsporte *registro)
{
    size_t i;

    if (registro == NULL) {
        return;
    }

    free(registro->nome);
    for (i = 0; i < registro->quantidade_caracteristicas; i++) {
        free(registro->caracteristicas[i]);
    }

    free(registro->caracteristicas);
    registro->nome = NULL;
    registro->caracteristicas = NULL;
    registro->quantidade_caracteristicas = 0;
}

void dados_esportes_destruir(DadosEsportes *dados)
{
    size_t i;

    if (dados == NULL) {
        return;
    }

    for (i = 0; i < dados->quantidade; i++) {
        registro_esporte_destruir(&dados->registros[i]);
    }

    free(dados->registros);
    dados->registros = NULL;
    dados->quantidade = 0;
}

static int adicionar_caracteristica(
    RegistroEsporte *registro,
    size_t *capacidade,
    char *caracteristica
)
{
    char **novo_vetor;
    size_t nova_capacidade;

    if (registro->quantidade_caracteristicas == *capacidade) {
        if (*capacidade > SIZE_MAX / 2) {
            return 0;
        }

        nova_capacidade = *capacidade == 0 ? 8 : *capacidade * 2;
        if (nova_capacidade > SIZE_MAX / sizeof(*novo_vetor)) {
            return 0;
        }

        novo_vetor = realloc(
            registro->caracteristicas,
            nova_capacidade * sizeof(*novo_vetor)
        );
        if (novo_vetor == NULL) {
            return 0;
        }

        registro->caracteristicas = novo_vetor;
        *capacidade = nova_capacidade;
    }

    registro->caracteristicas[registro->quantidade_caracteristicas] =
        caracteristica;
    registro->quantidade_caracteristicas++;
    return 1;
}

static int ler_caracteristicas(
    ParserJson *parser,
    RegistroEsporte *registro
)
{
    size_t capacidade = 0;
    char proximo;

    if (!parser_json_consumir_simbolo(parser, '[')) {
        return 0;
    }

    parser_json_ignorar_espacos(parser);
    if (
        parser_json_espiar(parser, &proximo) &&
        proximo == ']'
    ) {
        return parser_json_consumir_simbolo(parser, ']');
    }

    while (1) {
        char *caracteristica = parser_json_ler_string(parser);

        if (
            caracteristica == NULL ||
            caracteristica[0] == '\0' ||
            !adicionar_caracteristica(
                registro,
                &capacidade,
                caracteristica
            )
        ) {
            free(caracteristica);
            return 0;
        }

        parser_json_ignorar_espacos(parser);
        if (!parser_json_espiar(parser, &proximo)) {
            return 0;
        }

        if (proximo == ']') {
            return parser_json_consumir_simbolo(parser, ']');
        }

        if (!parser_json_consumir_simbolo(parser, ',')) {
            return 0;
        }
    }
}

static int ler_registro(
    ParserJson *parser,
    RegistroEsporte *registro
)
{
    int encontrou_esporte = 0;
    int encontrou_descricao = 0;
    char proximo;

    registro->nome = NULL;
    registro->caracteristicas = NULL;
    registro->quantidade_caracteristicas = 0;

    if (!parser_json_consumir_simbolo(parser, '{')) {
        return 0;
    }

    while (1) {
        char *chave = parser_json_ler_string(parser);
        int sucesso = 0;

        if (
            chave == NULL ||
            !parser_json_consumir_simbolo(parser, ':')
        ) {
            free(chave);
            return 0;
        }

        if (
            strcmp(chave, "esporte") == 0 &&
            !encontrou_esporte
        ) {
            registro->nome = parser_json_ler_string(parser);
            sucesso = registro->nome != NULL &&
                registro->nome[0] != '\0';
            encontrou_esporte = sucesso;
        } else if (
            strcmp(chave, "descricao") == 0 &&
            !encontrou_descricao
        ) {
            sucesso = ler_caracteristicas(parser, registro);
            encontrou_descricao = sucesso;
        }

        free(chave);
        if (!sucesso) {
            return 0;
        }

        parser_json_ignorar_espacos(parser);
        if (!parser_json_espiar(parser, &proximo)) {
            return 0;
        }

        if (proximo == '}') {
            if (!parser_json_consumir_simbolo(parser, '}')) {
                return 0;
            }
            break;
        }

        if (!parser_json_consumir_simbolo(parser, ',')) {
            return 0;
        }
    }

    return encontrou_esporte && encontrou_descricao;
}

static int adicionar_registro(
    DadosEsportes *dados,
    size_t *capacidade,
    RegistroEsporte *registro
)
{
    RegistroEsporte *novo_vetor;
    size_t nova_capacidade;

    if (dados->quantidade == *capacidade) {
        if (*capacidade > SIZE_MAX / 2) {
            return 0;
        }

        nova_capacidade = *capacidade == 0 ? 8 : *capacidade * 2;
        if (nova_capacidade > SIZE_MAX / sizeof(*novo_vetor)) {
            return 0;
        }

        novo_vetor = realloc(
            dados->registros,
            nova_capacidade * sizeof(*novo_vetor)
        );
        if (novo_vetor == NULL) {
            return 0;
        }

        dados->registros = novo_vetor;
        *capacidade = nova_capacidade;
    }

    dados->registros[dados->quantidade] = *registro;
    dados->quantidade++;
    registro->nome = NULL;
    registro->caracteristicas = NULL;
    registro->quantidade_caracteristicas = 0;
    return 1;
}

int dados_esportes_ler_json(
    const char *texto,
    size_t tamanho,
    DadosEsportes *dados
)
{
    ParserJson parser;
    DadosEsportes resultado = {NULL, 0};
    size_t capacidade = 0;
    char proximo;

    if (dados == NULL) {
        return 0;
    }

    dados->registros = NULL;
    dados->quantidade = 0;

    if (
        texto == NULL ||
        !parser_json_iniciar(&parser, texto, tamanho) ||
        !parser_json_consumir_simbolo(&parser, '[')
    ) {
        return 0;
    }

    parser_json_ignorar_espacos(&parser);
    if (
        parser_json_espiar(&parser, &proximo) &&
        proximo == ']'
    ) {
        if (
            !parser_json_consumir_simbolo(&parser, ']') ||
            !parser_json_finalizado(&parser)
        ) {
            return 0;
        }

        *dados = resultado;
        return 1;
    }

    while (1) {
        RegistroEsporte registro;

        if (
            !ler_registro(&parser, &registro) ||
            !adicionar_registro(
                &resultado,
                &capacidade,
                &registro
            )
        ) {
            registro_esporte_destruir(&registro);
            dados_esportes_destruir(&resultado);
            return 0;
        }

        parser_json_ignorar_espacos(&parser);
        if (!parser_json_espiar(&parser, &proximo)) {
            dados_esportes_destruir(&resultado);
            return 0;
        }

        if (proximo == ']') {
            if (!parser_json_consumir_simbolo(&parser, ']')) {
                dados_esportes_destruir(&resultado);
                return 0;
            }
            break;
        }

        if (!parser_json_consumir_simbolo(&parser, ',')) {
            dados_esportes_destruir(&resultado);
            return 0;
        }
    }

    if (!parser_json_finalizado(&parser)) {
        dados_esportes_destruir(&resultado);
        return 0;
    }

    *dados = resultado;
    return 1;
}
