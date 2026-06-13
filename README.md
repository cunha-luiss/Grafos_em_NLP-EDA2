# Grafos em NLP - EDA2

Projeto da disciplina Estruturas de Dados 2 da FCTE/UnB. O programa le um
arquivo JSON com esportes e caracteristicas e converte as strings em IDs
inteiros para a construcao posterior do grafo.

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
$env:Path = "C:\msys64\ucrt64\bin;$env:Path"
gcc -std=c17 -Wall -Wextra -Wpedantic -Iinclude src/*.c `
  -o build/grafos_nlp.exe
```

## Execucao

Usando o arquivo padrao:

```bash
make run
```

Ou informando outro arquivo:

```powershell
.\build\grafos_nlp.exe caminho\para\dados.json
```

```powershell
# Exemplo
.\build\grafos_nlp.exe "webscraper bruto.json"
```

Saida esperada para o arquivo atual:

```text
Esportes: 75
Caracteristicas unicas: 332
Vertices totais: 407
Associacoes esporte-caracteristica: 797
Grafo construido com sucesso!
Arvore Geradora Maxima construida com sucesso!
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

## Commits

Formato:

```text
<type>[optional scope]!: <short summary>
```

Exemplo:

```text
feat(parser): adiciona leitura de strings JSON
```

## Contribuicao

1. Crie uma branch a partir da `dev`.
2. Implemente e execute todos os testes.
3. Faca commits pequenos e objetivos.
4. Envie a branch para o repositorio remoto.
5. Abra um Pull Request para a branch `dev`.
