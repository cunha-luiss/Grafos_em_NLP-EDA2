#include "catalogo.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void testar_ids_sequenciais(void)
{
    Catalogo *catalogo = catalogo_criar(16);
    int id_atletismo;
    int id_corrida;

    assert(catalogo != NULL);
    assert(catalogo_obter_ou_inserir(
        catalogo,
        "Atletismo",
        TIPO_ESPORTE,
        &id_atletismo
    ));
    assert(catalogo_obter_ou_inserir(
        catalogo,
        "corrida",
        TIPO_CARACTERISTICA,
        &id_corrida
    ));

    assert(id_atletismo == 0);
    assert(id_corrida == 1);
    assert(catalogo_tamanho(catalogo) == 2);

    catalogo_destruir(catalogo);
}

static void testar_reutilizacao_de_id(void)
{
    Catalogo *catalogo = catalogo_criar(16);
    int primeiro_id;
    int segundo_id;

    assert(catalogo_obter_ou_inserir(
        catalogo,
        "bola",
        TIPO_CARACTERISTICA,
        &primeiro_id
    ));
    assert(catalogo_obter_ou_inserir(
        catalogo,
        "bola",
        TIPO_CARACTERISTICA,
        &segundo_id
    ));

    assert(primeiro_id == segundo_id);
    assert(catalogo_tamanho(catalogo) == 1);

    catalogo_destruir(catalogo);
}

static void testar_tipos_distintos(void)
{
    Catalogo *catalogo = catalogo_criar(16);
    int id_esporte;
    int id_caracteristica;

    assert(catalogo_obter_ou_inserir(
        catalogo,
        "atletismo",
        TIPO_ESPORTE,
        &id_esporte
    ));
    assert(catalogo_obter_ou_inserir(
        catalogo,
        "atletismo",
        TIPO_CARACTERISTICA,
        &id_caracteristica
    ));

    assert(id_esporte != id_caracteristica);
    assert(catalogo_tamanho(catalogo) == 2);

    catalogo_destruir(catalogo);
}

static void testar_busca_reversa(void)
{
    Catalogo *catalogo = catalogo_criar(16);
    TipoVertice tipo;
    int id;

    assert(catalogo_obter_ou_inserir(
        catalogo,
        "Futebol",
        TIPO_ESPORTE,
        &id
    ));
    assert(strcmp(catalogo_obter_texto(catalogo, id), "Futebol") == 0);
    assert(catalogo_obter_tipo(catalogo, id, &tipo));
    assert(tipo == TIPO_ESPORTE);

    assert(catalogo_obter_texto(catalogo, -1) == NULL);
    assert(catalogo_obter_texto(catalogo, 10) == NULL);

    catalogo_destruir(catalogo);
}

static void testar_expansao(void)
{
    Catalogo *catalogo = catalogo_criar(2);
    char texto[32];
    int id;
    int i;

    for (i = 0; i < 20; i++) {
        snprintf(texto, sizeof(texto), "caracteristica_%d", i);
        assert(catalogo_obter_ou_inserir(
            catalogo,
            texto,
            TIPO_CARACTERISTICA,
            &id
        ));
        assert(id == i);
    }

    assert(catalogo_tamanho(catalogo) == 20);
    catalogo_destruir(catalogo);
}

int main(void)
{
    testar_ids_sequenciais();
    testar_reutilizacao_de_id();
    testar_tipos_distintos();
    testar_busca_reversa();
    testar_expansao();

    puts("Todos os testes do catalogo passaram.");
    return 0;
}
