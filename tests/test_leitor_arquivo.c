#include "leitor_arquivo.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void testar_leitura_do_json(void)
{
    size_t tamanho;
    char *conteudo = ler_arquivo_texto(
        "data/webscraper_padronizado.json",
        &tamanho
    );

    assert(conteudo != NULL);
    assert(tamanho > 0);
    assert(conteudo[0] == '[');
    assert(conteudo[tamanho] == '\0');
    assert(strstr(conteudo, "\"esporte\"") != NULL);

    free(conteudo);
}

static void testar_arquivo_vazio(void)
{
    const char *caminho = "build/teste_arquivo_vazio.tmp";
    FILE *arquivo = fopen(caminho, "wb");
    size_t tamanho = 99;
    char *conteudo;

    assert(arquivo != NULL);
    assert(fclose(arquivo) == 0);

    conteudo = ler_arquivo_texto(caminho, &tamanho);

    assert(conteudo != NULL);
    assert(tamanho == 0);
    assert(conteudo[0] == '\0');

    free(conteudo);
    assert(remove(caminho) == 0);
}

static void testar_arquivo_inexistente(void)
{
    size_t tamanho = 99;
    char *conteudo = ler_arquivo_texto(
        "data/arquivo_inexistente.json",
        &tamanho
    );

    assert(conteudo == NULL);
    assert(tamanho == 0);
}

static void testar_parametros_invalidos(void)
{
    size_t tamanho;

    assert(ler_arquivo_texto(NULL, &tamanho) == NULL);
    assert(ler_arquivo_texto(
        "data/webscraper_padronizado.json",
        NULL
    ) == NULL);
}

int main(void)
{
    testar_leitura_do_json();
    testar_arquivo_vazio();
    testar_arquivo_inexistente();
    testar_parametros_invalidos();

    puts("Todos os testes do leitor de arquivo passaram.");
    return 0;
}
