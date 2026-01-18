# DC-MST - Degree-Constrained Minimum Spanning Tree

Trabalho da disciplina Teoria dos Grafos - UFJF

## Problema

Dado um grafo conexo ponderado G = (V, E, w) e um limite de grau máximo d,
o problema DC-MST consiste em encontrar uma árvore geradora de custo mínimo
tal que nenhum vértice tenha grau superior a d.

## Estrutura do Projeto

```
trb2-grafos/
├── src/           # Código fonte (.cpp)
├── include/       # Headers (.h)
├── instancias/    # Instâncias de teste
├── resultados/    # Saída dos experimentos
└── build/         # Arquivos compilados
```

## Compilação

```bash
make
```

## Execução

Após compilar o projeto, execute o programa com o seguinte formato:

```bash
./programa <algoritmo> <arquivo_instancia> [parametros]
```

- `<algoritmo>`: Algoritmo a ser utilizado (`guloso`, `randomizado` ou `reativo`)
- `<arquivo_instancia>`: Nome do arquivo de instância (ex: `crd100`).  
  **Observação:** `crd100` é apenas um exemplo; você pode utilizar qualquer outro arquivo de instância disponível, bastando informar o nome correspondente.
- `[parametros]`: Parâmetros adicionais, dependendo do algoritmo escolhido

### Exemplos

**Guloso**

```bash
./programa guloso crd100 1
```

- `1`: Limite de grau máximo dos vértices

**Randomizado**

```bash
./programa randomizado crd100 0.3 30
```

- `0.3`: Parâmetro alfa (controle de aleatoriedade)
- `30`: Limite de grau máximo dos vértices

**Reativo**

```bash
./programa reativo crd100 300 30
```

- `300`: Número de iterações totais
- `30`: Limite de grau máximo dos vértices


## Integrantes

- Vitor de Souza Reis - 202376036
- Lara da Silva Dias - 202376010
- Luiza Caldeira Daniel - 202376012
- Sarah Cristina Freitas da Silva - 202376034
