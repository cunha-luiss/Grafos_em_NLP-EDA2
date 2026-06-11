#include "parser_json.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void testar_inicializacao(void)
{
    const char *texto = "[]";
    ParserJson parser;

    assert(parser_json_iniciar(&parser, texto, strlen(texto)));
    assert(parser_json_posicao(&parser) == 0);
    assert(!parser_json_finalizado(&parser));
}

static void testar_espacos_em_branco(void)
{
    const char *texto = " \t\n\r[";
    ParserJson parser;
    char caractere;

    assert(parser_json_iniciar(&parser, texto, strlen(texto)));
    parser_json_ignorar_espacos(&parser);

    assert(parser_json_posicao(&parser) == 4);
    assert(parser_json_espiar(&parser, &caractere));
    assert(caractere == '[');
}

static void testar_consumo_de_simbolos(void)
{
    const char *texto = " [ { } , : ] ";
    const char simbolos[] = {'[', '{', '}', ',', ':', ']'};
    ParserJson parser;
    size_t i;

    assert(parser_json_iniciar(&parser, texto, strlen(texto)));

    for (i = 0; i < sizeof(simbolos); i++) {
        assert(parser_json_consumir_simbolo(&parser, simbolos[i]));
    }

    assert(parser_json_finalizado(&parser));
}

static void testar_simbolo_inesperado(void)
{
    const char *texto = "  {";
    ParserJson parser;

    assert(parser_json_iniciar(&parser, texto, strlen(texto)));
    assert(!parser_json_consumir_simbolo(&parser, '['));
    assert(parser_json_posicao(&parser) == 2);
    assert(parser_json_consumir_simbolo(&parser, '{'));
}

static void testar_fim_do_texto(void)
{
    const char *texto = "[]";
    ParserJson parser;
    char caractere;

    assert(parser_json_iniciar(&parser, texto, strlen(texto)));
    assert(parser_json_consumir_simbolo(&parser, '['));
    assert(parser_json_consumir_simbolo(&parser, ']'));
    assert(parser_json_finalizado(&parser));
    assert(!parser_json_espiar(&parser, &caractere));
    assert(!parser_json_consumir_simbolo(&parser, '}'));
}

static void testar_parametros_invalidos(void)
{
    ParserJson parser;

    assert(!parser_json_iniciar(NULL, "[]", 2));
    assert(!parser_json_iniciar(&parser, NULL, 0));
    assert(!parser_json_consumir_simbolo(NULL, '['));
    assert(parser_json_iniciar(&parser, "[]", 2));
    assert(!parser_json_consumir_simbolo(&parser, '"'));
    assert(!parser_json_finalizado(NULL));
}

int main(void)
{
    testar_inicializacao();
    testar_espacos_em_branco();
    testar_consumo_de_simbolos();
    testar_simbolo_inesperado();
    testar_fim_do_texto();
    testar_parametros_invalidos();

    puts("Todos os testes da base do parser JSON passaram.");
    return 0;
}
