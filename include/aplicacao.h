#ifndef APLICACAO_H
#define APLICACAO_H

#include <stdio.h>

int aplicacao_executar(
    const char *caminho_json,
    FILE *saida,
    FILE *saida_erros
);

#endif
