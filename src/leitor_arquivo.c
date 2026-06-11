#include "leitor_arquivo.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

char *ler_arquivo_texto(const char *caminho, size_t *tamanho)
{
    FILE *arquivo;
    long tamanho_arquivo;
    size_t bytes_lidos;
    char *conteudo;

    if (caminho == NULL || tamanho == NULL) {
        return NULL;
    }

    *tamanho = 0;
    arquivo = fopen(caminho, "rb");
    if (arquivo == NULL) {
        return NULL;
    }

    if (fseek(arquivo, 0, SEEK_END) != 0) {
        fclose(arquivo);
        return NULL;
    }

    tamanho_arquivo = ftell(arquivo);
    if (
        tamanho_arquivo < 0 ||
        (uintmax_t)tamanho_arquivo > SIZE_MAX - 1
    ) {
        fclose(arquivo);
        return NULL;
    }

    if (fseek(arquivo, 0, SEEK_SET) != 0) {
        fclose(arquivo);
        return NULL;
    }

    conteudo = malloc((size_t)tamanho_arquivo + 1);
    if (conteudo == NULL) {
        fclose(arquivo);
        return NULL;
    }

    bytes_lidos = fread(
        conteudo,
        1,
        (size_t)tamanho_arquivo,
        arquivo
    );

    if (
        bytes_lidos != (size_t)tamanho_arquivo ||
        ferror(arquivo)
    ) {
        free(conteudo);
        fclose(arquivo);
        return NULL;
    }

    conteudo[bytes_lidos] = '\0';
    *tamanho = bytes_lidos;
    fclose(arquivo);
    return conteudo;
}
