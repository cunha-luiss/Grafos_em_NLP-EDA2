#include "parser_json.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
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

static void testar_string_simples(void)
{
    const char *texto = "  \"Atletismo\" ,";
    ParserJson parser;
    char *resultado;

    assert(parser_json_iniciar(&parser, texto, strlen(texto)));
    resultado = parser_json_ler_string(&parser);

    assert(resultado != NULL);
    assert(strcmp(resultado, "Atletismo") == 0);
    assert(parser_json_consumir_simbolo(&parser, ','));

    free(resultado);
}

static void testar_escapes(void)
{
    const char *texto = "\"linha\\naspas: \\\" barra: \\\\\\/\\t\"";
    const char *esperado = "linha\naspas: \" barra: \\/\t";
    ParserJson parser;
    char *resultado;

    assert(parser_json_iniciar(&parser, texto, strlen(texto)));
    resultado = parser_json_ler_string(&parser);

    assert(resultado != NULL);
    assert(strcmp(resultado, esperado) == 0);
    assert(parser_json_finalizado(&parser));

    free(resultado);
}

static void testar_unicode(void)
{
    const char *texto = "\"lan\\u00e7amento \\ud83c\\udfc5\"";
    const char esperado[] = {
        'l', 'a', 'n', (char)0xC3, (char)0xA7, 'a', 'm', 'e', 'n', 't', 'o',
        ' ', (char)0xF0, (char)0x9F, (char)0x8F, (char)0x85, '\0'
    };
    ParserJson parser;
    char *resultado;

    assert(parser_json_iniciar(&parser, texto, strlen(texto)));
    resultado = parser_json_ler_string(&parser);

    assert(resultado != NULL);
    assert(strcmp(resultado, esperado) == 0);

    free(resultado);
}

static void testar_string_invalida(void)
{
    const char *casos[] = {
        "\"sem fechamento",
        "\"escape \\x invalido\"",
        "\"unicode \\u12xz\"",
        "\"nulo \\u0000 invalido\"",
        "\"substituto \\ud800 invalido\"",
        "\"substituto baixo \\udc00\"",
        "\"quebra\ninvalida\""
    };
    ParserJson parser;
    size_t i;

    for (i = 0; i < sizeof(casos) / sizeof(casos[0]); i++) {
        assert(parser_json_iniciar(
            &parser,
            casos[i],
            strlen(casos[i])
        ));
        assert(parser_json_ler_string(&parser) == NULL);
        assert(parser_json_posicao(&parser) == 0);
    }
}

static void testar_parametros_invalidos(void)
{
    ParserJson parser;

    assert(!parser_json_iniciar(NULL, "[]", 2));
    assert(!parser_json_iniciar(&parser, NULL, 0));
    assert(!parser_json_consumir_simbolo(NULL, '['));
    assert(parser_json_iniciar(&parser, "[]", 2));
    assert(!parser_json_consumir_simbolo(&parser, '"'));
    assert(parser_json_ler_string(NULL) == NULL);
    assert(parser_json_ler_string(&parser) == NULL);
    assert(parser_json_posicao(&parser) == 0);
    assert(!parser_json_finalizado(NULL));
}

int main(void)
{
    testar_inicializacao();
    testar_espacos_em_branco();
    testar_consumo_de_simbolos();
    testar_simbolo_inesperado();
    testar_fim_do_texto();
    testar_string_simples();
    testar_escapes();
    testar_unicode();
    testar_string_invalida();
    testar_parametros_invalidos();

    puts("Todos os testes de strings JSON passaram.");
    return 0;
}
