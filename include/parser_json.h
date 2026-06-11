#ifndef PARSER_JSON_H
#define PARSER_JSON_H

#include <stddef.h>

typedef struct {
    const char *texto;
    size_t tamanho;
    size_t posicao;
} ParserJson;

int parser_json_iniciar(
    ParserJson *parser,
    const char *texto,
    size_t tamanho
);

void parser_json_ignorar_espacos(ParserJson *parser);
int parser_json_espiar(const ParserJson *parser, char *caractere);
int parser_json_consumir_simbolo(ParserJson *parser, char simbolo);
char *parser_json_ler_string(ParserJson *parser);
int parser_json_finalizado(const ParserJson *parser);
size_t parser_json_posicao(const ParserJson *parser);

#endif
