# Grafos em NLP - EDA2 2026.01

Criação de um algoritmo de Processamento de Linguaggem Natural usando os algoritmos tradicionais (Kruskal, grafos ponderados, hash e heap) em grafos para a matéria EDA2 (2026.1) da FCTE/UnB

## Requisitos

- C17 ou C11 (alterar para o seu em "CFLAGS" no Make)
- GCC
- Make, opcional

As estruturas de dados e o parser JSON foram implementados pelo grupo.

## Estrutura

```text
build/                          Binarios gerados
data/                           Arquivos de entrada
include/                        Cabecalhos publicos
preProcessingPython/            Pasta com o Pré Processamento feito para pegar dados do Wikipedia usando spaCy em Python
src/                            Codigo-fonte
tests/                          Testes automatizados
SAIDA OBTIDA PELO GRUPO.txt     Arquivo com a saída analisada pelo grupo
slidesEDA2TemaD.pdf             Slides para apresentação da equipe sobre o projeto em formato PDF
slidesEDA2TemaD.pptx            Slides para apresentação da equipe sobre o projeto
```

## Compilação

O grupo compilou o projeto usando o terminal do linux

No terminal MSYS2 UCRT64:

```bash
make
```

No PowerShell:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src/*.c -o build/grafos_nlp.exe
```

## Execução

informando um arquivo e gerando um arquivo de saida:

```powershell
./build/grafos_nlp.exe data/webscraper_padronizado.json > saida.txt
```

Saida esperada para o arquivo atual:

```text
Esportes: 75
Caracteristicas unicas: 316
Vertices totais: 391
Associacoes esporte-caracteristica: 1077
Grafo construido com sucesso!
Arvore Geradora Maxima construida com sucesso!
Comparacao esporte-esporte: minimo de 3 caracteristicas em comum (resgate de isolados com 2).

Comunidades detectadas: 18
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

## Pre-processamento

O arquivo `data/webscraper_padronizado.json` foi limpo para remover termos
genericos e enriquecido com algumas tags semanticas controladas. Para reaplicar
essas tags:

```powershell
python preProcessingPython\enriquecer_tags_semanticas.py
```

## Contrato Para O Grafo

O modulo `dados_identificados` entrega:

- um catalogo que converte texto e tipo em ID;
- consulta reversa de ID para texto e tipo;
- uma lista de esportes identificados;
- para cada esporte, um vetor com os IDs de suas caracteristicas.

Esportes e caracteristicas usam IDs globais, mas tipos distintos. Assim,
o esporte `Atletismo` e a caracteristica `atletismo` sao vertices diferentes.

Para detectar comunidades, o programa monta um grafo adicional ligando
diretamente dois esportes quando eles compartilham pelo menos 3
caracteristicas unicas. O peso dessa aresta e a quantidade de
caracteristicas em comum.
