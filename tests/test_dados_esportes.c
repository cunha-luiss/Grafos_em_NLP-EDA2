#include "dados_esportes.h"
#include "leitor_arquivo.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void testar_json_simples(void)
{
    const char *json =
        "["
        "{\"esporte\":\"Basquete\","
        "\"descricao\":[\"bola\",\"quadra\"]},"
        "{\"descricao\":[\"agua\"],"
        "\"esporte\":\"Natacao\"}"
        "]";
    DadosEsportes dados;

    assert(dados_esportes_ler_json(json, strlen(json), &dados));
    assert(dados.quantidade == 2);
    assert(strcmp(dados.registros[0].nome, "Basquete") == 0);
    assert(dados.registros[0].quantidade_caracteristicas == 2);
    assert(strcmp(
        dados.registros[0].caracteristicas[1],
        "quadra"
    ) == 0);
    assert(strcmp(dados.registros[1].nome, "Natacao") == 0);

    dados_esportes_destruir(&dados);
}

static void testar_json_real(void)
{
    size_t tamanho;
    size_t total_caracteristicas = 0;
    size_t i;
    char *json = ler_arquivo_texto(
        "data/webscraper_padronizado.json",
        &tamanho
    );
    DadosEsportes dados;

    assert(json != NULL);
    assert(dados_esportes_ler_json(json, tamanho, &dados));
    assert(dados.quantidade == 75);
    assert(strcmp(dados.registros[0].nome, "Atletismo") == 0);
    assert(strcmp(dados.registros[74].nome, "Wushu") == 0);

    for (i = 0; i < dados.quantidade; i++) {
        total_caracteristicas +=
            dados.registros[i].quantidade_caracteristicas;
    }
    assert(total_caracteristicas == 1536);

    dados_esportes_destruir(&dados);
    free(json);
}

static void testar_array_vazio(void)
{
    const char *json = " [ ] ";
    DadosEsportes dados;

    assert(dados_esportes_ler_json(json, strlen(json), &dados));
    assert(dados.quantidade == 0);
    assert(dados.registros == NULL);

    dados_esportes_destruir(&dados);
}

static void testar_jsons_invalidos(void)
{
    const char *casos[] = {
        "{}",
        "[{\"esporte\":\"Futebol\"}]",
        "[{\"descricao\":[\"bola\"]}]",
        "[{\"esporte\":\"\",\"descricao\":[]}]",
        "[{\"esporte\":\"Futebol\",\"descricao\":[\"\"]}]",
        "[{\"esporte\":\"Futebol\",\"descricao\":{}}]",
        "[{\"esporte\":\"Futebol\",\"extra\":\"x\","
            "\"descricao\":[]}]",
        "[{\"esporte\":\"Futebol\",\"esporte\":\"Futsal\","
            "\"descricao\":[]}]",
        "[{\"esporte\":\"Futebol\",\"descricao\":[],}]",
        "[{\"esporte\":\"Futebol\",\"descricao\":[]}],"
        "[{\"esporte\":\"Futebol\",\"descricao\":[]},]"
    };
    size_t i;

    for (i = 0; i < sizeof(casos) / sizeof(casos[0]); i++) {
        DadosEsportes dados = {NULL, 0};

        assert(!dados_esportes_ler_json(
            casos[i],
            strlen(casos[i]),
            &dados
        ));
        assert(dados.registros == NULL);
        assert(dados.quantidade == 0);
    }
}

static void testar_parametros_invalidos(void)
{
    DadosEsportes dados = {NULL, 0};

    assert(!dados_esportes_ler_json(NULL, 0, &dados));
    assert(!dados_esportes_ler_json("[]", 2, NULL));
    dados_esportes_destruir(NULL);
}

int main(void)
{
    testar_json_simples();
    testar_json_real();
    testar_array_vazio();
    testar_jsons_invalidos();
    testar_parametros_invalidos();

    puts("Todos os testes dos dados de esportes passaram.");
    return 0;
}
