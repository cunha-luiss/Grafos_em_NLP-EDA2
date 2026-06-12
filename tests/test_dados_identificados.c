#include "dados_esportes.h"
#include "dados_identificados.h"
#include "leitor_arquivo.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void testar_identificacao_simples(void)
{
    const char *json =
        "["
        "{\"esporte\":\"Basquete\","
        "\"descricao\":[\"bola\",\"equipe\"]},"
        "{\"esporte\":\"Futebol\","
        "\"descricao\":[\"bola\",\"campo\"]}"
        "]";
    DadosEsportes origem;
    DadosIdentificados dados;
    TipoVertice tipo;
    int id_bola;

    assert(dados_esportes_ler_json(json, strlen(json), &origem));
    assert(dados_identificados_criar(&origem, &dados));

    assert(dados.quantidade_esportes == 2);
    assert(catalogo_tamanho(dados.catalogo) == 5);
    assert(dados.esportes[0].id_esporte == 0);
    assert(dados.esportes[0].quantidade_caracteristicas == 2);
    assert(dados.esportes[1].id_esporte == 3);

    assert(catalogo_buscar(
        dados.catalogo,
        "bola",
        TIPO_CARACTERISTICA,
        &id_bola
    ));
    assert(dados.esportes[0].ids_caracteristicas[0] == id_bola);
    assert(dados.esportes[1].ids_caracteristicas[0] == id_bola);

    assert(catalogo_obter_tipo(
        dados.catalogo,
        dados.esportes[0].id_esporte,
        &tipo
    ));
    assert(tipo == TIPO_ESPORTE);

    dados_identificados_destruir(&dados);
    dados_esportes_destruir(&origem);
}

static void testar_mesmo_texto_em_tipos_distintos(void)
{
    const char *json =
        "[{\"esporte\":\"atletismo\","
        "\"descricao\":[\"atletismo\"]}]";
    DadosEsportes origem;
    DadosIdentificados dados;

    assert(dados_esportes_ler_json(json, strlen(json), &origem));
    assert(dados_identificados_criar(&origem, &dados));

    assert(dados.esportes[0].id_esporte !=
        dados.esportes[0].ids_caracteristicas[0]);
    assert(catalogo_tamanho(dados.catalogo) == 2);

    dados_identificados_destruir(&dados);
    dados_esportes_destruir(&origem);
}

static void testar_json_real(void)
{
    size_t tamanho;
    char *json = ler_arquivo_texto(
        "data/webscraper_padronizado.json",
        &tamanho
    );
    DadosEsportes origem;
    DadosIdentificados dados;
    int id_esporte;
    int id_caracteristica;

    assert(json != NULL);
    assert(dados_esportes_ler_json(json, tamanho, &origem));
    assert(dados_identificados_criar(&origem, &dados));

    assert(dados.quantidade_esportes == 75);
    assert(catalogo_tamanho(dados.catalogo) == 407);
    assert(catalogo_buscar(
        dados.catalogo,
        "Atletismo",
        TIPO_ESPORTE,
        &id_esporte
    ));
    assert(catalogo_buscar(
        dados.catalogo,
        "atletismo",
        TIPO_CARACTERISTICA,
        &id_caracteristica
    ));
    assert(id_esporte != id_caracteristica);

    dados_identificados_destruir(&dados);
    dados_esportes_destruir(&origem);
    free(json);
}

static void testar_origem_vazia(void)
{
    DadosEsportes origem = {NULL, 0};
    DadosIdentificados dados;

    assert(dados_identificados_criar(&origem, &dados));
    assert(dados.quantidade_esportes == 0);
    assert(dados.esportes == NULL);
    assert(catalogo_tamanho(dados.catalogo) == 0);

    dados_identificados_destruir(&dados);
}

static void testar_parametros_invalidos(void)
{
    DadosEsportes origem = {NULL, 0};
    DadosEsportes origem_invalida = {NULL, 1};
    DadosIdentificados dados;

    assert(!dados_identificados_criar(NULL, &dados));
    assert(!dados_identificados_criar(&origem, NULL));
    assert(!dados_identificados_criar(&origem_invalida, &dados));
    assert(dados.catalogo == NULL);
    assert(dados.esportes == NULL);
    assert(dados.quantidade_esportes == 0);
    dados_identificados_destruir(NULL);
}

int main(void)
{
    testar_identificacao_simples();
    testar_mesmo_texto_em_tipos_distintos();
    testar_json_real();
    testar_origem_vazia();
    testar_parametros_invalidos();

    puts("Todos os testes de identificacao passaram.");
    return 0;
}
