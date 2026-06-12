#include "aplicacao.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    const char *caminho_json = "data/webscraper_padronizado.json";

    if (argc > 2) {
        fprintf(stderr, "Uso: %s [caminho_do_json]\n", argv[0]);
        return 1;
    }

    if (argc == 2) {
        caminho_json = argv[1];
    }

    return aplicacao_executar(caminho_json, stdout, stderr);
}
