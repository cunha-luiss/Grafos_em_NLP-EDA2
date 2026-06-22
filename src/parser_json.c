#include "parser_json.h"

#include <stdint.h>
#include <stdlib.h>

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

static int valor_hexadecimal(char caractere, uint32_t *valor)
{
    if (caractere >= '0' && caractere <= '9') {
        *valor = (uint32_t)(caractere - '0');
        return 1;
    }

    if (caractere >= 'a' && caractere <= 'f') {
        *valor = (uint32_t)(caractere - 'a' + 10);
        return 1;
    }

    if (caractere >= 'A' && caractere <= 'F') {
        *valor = (uint32_t)(caractere - 'A' + 10);
        return 1;
    }

    return 0;
}

static int ler_codigo_unicode(
    const ParserJson *parser,
    size_t *posicao,
    uint32_t *codigo
)
{
    uint32_t valor;
    size_t i;

    if (parser->tamanho - *posicao < 4) {
        return 0;
    }

    *codigo = 0;
    for (i = 0; i < 4; i++) {
        if (!valor_hexadecimal(parser->texto[*posicao], &valor)) {
            return 0;
        }

        *codigo = (*codigo << 4) | valor;
        (*posicao)++;
    }

    return 1;
}

static size_t escrever_utf8(char *destino, uint32_t codigo)
{
    if (codigo <= UINT32_C(0x7F)) {
        destino[0] = (char)codigo;
        return 1;
    }

    if (codigo <= UINT32_C(0x7FF)) {
        destino[0] = (char)(UINT32_C(0xC0) | (codigo >> 6));
        destino[1] = (char)(UINT32_C(0x80) | (codigo & UINT32_C(0x3F)));
        return 2;
    }

    if (codigo <= UINT32_C(0xFFFF)) {
        destino[0] = (char)(UINT32_C(0xE0) | (codigo >> 12));
        destino[1] = (char)(
            UINT32_C(0x80) |
            ((codigo >> 6) & UINT32_C(0x3F))
        );
        destino[2] = (char)(UINT32_C(0x80) | (codigo & UINT32_C(0x3F)));
        return 3;
    }

    destino[0] = (char)(UINT32_C(0xF0) | (codigo >> 18));
    destino[1] = (char)(
        UINT32_C(0x80) |
        ((codigo >> 12) & UINT32_C(0x3F))
    );
    destino[2] = (char)(
        UINT32_C(0x80) |
        ((codigo >> 6) & UINT32_C(0x3F))
    );
    destino[3] = (char)(UINT32_C(0x80) | (codigo & UINT32_C(0x3F)));
    return 4;
}

static int decodificar_unicode(
    const ParserJson *parser,
    size_t *posicao,
    char *destino,
    size_t *quantidade
)
{
    uint32_t codigo;
    uint32_t codigo_baixo;

    if (!ler_codigo_unicode(parser, posicao, &codigo)) {
        return 0;
    }

    if (codigo == 0) {
        return 0;
    }

    if (codigo >= UINT32_C(0xDC00) && codigo <= UINT32_C(0xDFFF)) {
        return 0;
    }

    if (codigo >= UINT32_C(0xD800) && codigo <= UINT32_C(0xDBFF)) {
        if (
            parser->tamanho - *posicao < 6 ||
            parser->texto[*posicao] != '\\' ||
            parser->texto[*posicao + 1] != 'u'
        ) {
            return 0;
        }

        *posicao += 2;
        if (
            !ler_codigo_unicode(parser, posicao, &codigo_baixo) ||
            codigo_baixo < UINT32_C(0xDC00) ||
            codigo_baixo > UINT32_C(0xDFFF)
        ) {
            return 0;
        }

        codigo = UINT32_C(0x10000) +
            ((codigo - UINT32_C(0xD800)) << 10) +
            (codigo_baixo - UINT32_C(0xDC00));
    }

    *quantidade = escrever_utf8(destino, codigo);
    return 1;
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

char *parser_json_ler_string(ParserJson *parser)
{
    size_t inicio;
    size_t leitura;
    size_t escrita = 0;
    char *resultado;

    if (parser == NULL || parser->texto == NULL) {
        return NULL;
    }

    inicio = parser->posicao;
    parser_json_ignorar_espacos(parser);

    if (
        parser->posicao >= parser->tamanho ||
        parser->texto[parser->posicao] != '"'
    ) {
        parser->posicao = inicio;
        return NULL;
    }

    leitura = parser->posicao + 1;
    resultado = malloc(parser->tamanho - leitura + 1);
    if (resultado == NULL) {
        parser->posicao = inicio;
        return NULL;
    }

    while (leitura < parser->tamanho) {
        unsigned char caractere = (unsigned char)parser->texto[leitura++];

        if (caractere == '"') {
            resultado[escrita] = '\0';
            parser->posicao = leitura;
            return resultado;
        }

        if (caractere < 0x20) {
            break;
        }

        if (caractere != '\\') {
            resultado[escrita++] = (char)caractere;
            continue;
        }

        if (leitura >= parser->tamanho) {
            break;
        }

        caractere = (unsigned char)parser->texto[leitura++];
        switch (caractere) {
            case '"':
            case '\\':
            case '/':
                resultado[escrita++] = (char)caractere;
                break;
            case 'b':
                resultado[escrita++] = '\b';
                break;
            case 'f':
                resultado[escrita++] = '\f';
                break;
            case 'n':
                resultado[escrita++] = '\n';
                break;
            case 'r':
                resultado[escrita++] = '\r';
                break;
            case 't':
                resultado[escrita++] = '\t';
                break;
            case 'u': {
                size_t quantidade;

                if (!decodificar_unicode(
                    parser,
                    &leitura,
                    resultado + escrita,
                    &quantidade
                )) {
                    free(resultado);
                    parser->posicao = inicio;
                    return NULL;
                }

                escrita += quantidade;
                break;
            }
            default:
                free(resultado);
                parser->posicao = inicio;
                return NULL;
        }
    }

    free(resultado);
    parser->posicao = inicio;
    return NULL;
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
