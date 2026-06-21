# Grafos em NLP - EDA2

Criação de um algoritmo de Processamento de Linguaggem Natural usando os algoritmos tradicionais (Kruskal, grafos ponderados, hash e heap) em grafos para a matéria EDA2 (2026.1) da FCTE/UnB

## Requisitos

- C17 ou C11 (alterar para o seu em "CFLAGS" no Make)
- GCC
- Make, opcional

As estruturas de dados e o parser JSON foram implementados pelo grupo.

## Estrutura

```text
data/       Arquivos de entrada
include/    Cabecalhos publicos
src/        Codigo-fonte
tests/      Testes automatizados
build/      Binarios gerados
```

## Compilacao

No terminal MSYS2 UCRT64:

```bash
make
```

No PowerShell:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src/*.c -o build/grafos_nlp.exe
```

## Execucao

informando um arquivo:

```powershell
./build/grafos_nlp.exe data/webscraper_padronizado.json
```

Saida esperada para o arquivo atual:

```text
Esportes: 75
Caracteristicas unicas: 332
Vertices totais: 407
Associacoes esporte-caracteristica: 797
Grafo construido com sucesso!
Arvore Geradora Maxima construida com sucesso!

Comunidades:......
```

## Testes

No PowerShell:

```powershell
.\scripts\testar.ps1
```

Com Make instalado:

```bash
make test
```

Os testes cobrem tabela hash, catalogo de IDs, leitura de arquivo, parser JSON,
extracao dos esportes e integracao completa da aplicacao.

## Contrato Para O Grafo

O modulo `dados_identificados` entrega:

- um catalogo que converte texto e tipo em ID;
- consulta reversa de ID para texto e tipo;
- uma lista de esportes identificados;
- para cada esporte, um vetor com os IDs de suas caracteristicas.

Esportes e caracteristicas usam IDs globais, mas tipos distintos. Assim,
o esporte `Atletismo` e a caracteristica `atletismo` sao vertices diferentes.

