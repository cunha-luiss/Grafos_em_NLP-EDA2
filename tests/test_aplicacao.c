#include "aplicacao.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static int arquivo_contem(FILE *arquivo, const char *trecho)
{
    char buffer[1024];
    size_t quantidade;

    rewind(arquivo);
    quantidade = fread(buffer, 1, sizeof(buffer) - 1, arquivo);
    buffer[quantidade] = '\0';
    return strstr(buffer, trecho) != NULL;
}

static void testar_pipeline_completo(void)
{
    FILE *saida = tmpfile();
    FILE *erros = tmpfile();

    assert(saida != NULL);
    assert(erros != NULL);
    assert(aplicacao_executar(
        "data/webscraper_padronizado.json",
        saida,
        erros
    ) == 0);

    assert(arquivo_contem(saida, "Esportes: 75"));
    assert(arquivo_contem(saida, "Caracteristicas unicas: 332"));
    assert(arquivo_contem(saida, "Vertices totais: 407"));
    assert(arquivo_contem(
        saida,
        "Associacoes esporte-caracteristica: 797"
    ));

    assert(fclose(saida) == 0);
    assert(fclose(erros) == 0);
}

static void testar_arquivo_inexistente(void)
{
    FILE *saida = tmpfile();
    FILE *erros = tmpfile();

    assert(saida != NULL);
    assert(erros != NULL);
    assert(aplicacao_executar(
        "data/arquivo_inexistente.json",
        saida,
        erros
    ) == 1);
    assert(arquivo_contem(
        erros,
        "nao foi possivel ler o arquivo"
    ));

    assert(fclose(saida) == 0);
    assert(fclose(erros) == 0);
}

static void testar_json_invalido(void)
{
    const char *caminho = "build/teste_json_invalido.tmp";
    FILE *arquivo = fopen(caminho, "wb");
    FILE *saida;
    FILE *erros;

    assert(arquivo != NULL);
    assert(fputs("{}", arquivo) >= 0);
    assert(fclose(arquivo) == 0);

    saida = tmpfile();
    erros = tmpfile();
    assert(saida != NULL);
    assert(erros != NULL);
    assert(aplicacao_executar(caminho, saida, erros) == 1);
    assert(arquivo_contem(erros, "nao possui o formato esperado"));

    assert(fclose(saida) == 0);
    assert(fclose(erros) == 0);
    assert(remove(caminho) == 0);
}

static void testar_parametros_invalidos(void)
{
    FILE *arquivo = tmpfile();

    assert(arquivo != NULL);
    assert(aplicacao_executar(NULL, arquivo, arquivo) == 1);
    assert(aplicacao_executar("dados.json", NULL, arquivo) == 1);
    assert(aplicacao_executar("dados.json", arquivo, NULL) == 1);
    assert(fclose(arquivo) == 0);
}

int main(void)
{
    testar_pipeline_completo();
    testar_arquivo_inexistente();
    testar_json_invalido();
    testar_parametros_invalidos();

    puts("Todos os testes da aplicacao passaram.");
    return 0;
}
