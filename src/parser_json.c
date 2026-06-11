#include "parser_json.h"

static int eh_espaco_json(char caractere)
{
    return (
        caractere == ' ' ||
        caractere == '\t' ||
        caractere == '\n' ||
        caractere == '\r'
    );
}

static int eh_simbolo_json(char caractere)
{
    return (
        caractere == '{' ||
        caractere == '}' ||
        caractere == '[' ||
        caractere == ']' ||
        caractere == ':' ||
        caractere == ','
    );
}

int parser_json_iniciar(
    ParserJson *parser,
    const char *texto,
    size_t tamanho
)
{
    if (parser == NULL || texto == NULL) {
        return 0;
    }

    parser->texto = texto;
    parser->tamanho = tamanho;
    parser->posicao = 0;
    return 1;
}

void parser_json_ignorar_espacos(ParserJson *parser)
{
    if (parser == NULL || parser->texto == NULL) {
        return;
    }

    while (
        parser->posicao < parser->tamanho &&
        eh_espaco_json(parser->texto[parser->posicao])
    ) {
        parser->posicao++;
    }
}

int parser_json_espiar(const ParserJson *parser, char *caractere)
{
    if (
        parser == NULL ||
        parser->texto == NULL ||
        caractere == NULL ||
        parser->posicao >= parser->tamanho
    ) {
        return 0;
    }

    *caractere = parser->texto[parser->posicao];
    return 1;
}

int parser_json_consumir_simbolo(ParserJson *parser, char simbolo)
{
    if (
        parser == NULL ||
        parser->texto == NULL ||
        !eh_simbolo_json(simbolo)
    ) {
        return 0;
    }

    parser_json_ignorar_espacos(parser);

    if (
        parser->posicao >= parser->tamanho ||
        parser->texto[parser->posicao] != simbolo
    ) {
        return 0;
    }

    parser->posicao++;
    return 1;
}

int parser_json_finalizado(const ParserJson *parser)
{
    ParserJson copia;

    if (parser == NULL || parser->texto == NULL) {
        return 0;
    }

    copia = *parser;
    parser_json_ignorar_espacos(&copia);
    return copia.posicao == copia.tamanho;
}

size_t parser_json_posicao(const ParserJson *parser)
{
    if (parser == NULL) {
        return 0;
    }

    return parser->posicao;
}
