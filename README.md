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

```bash
./programa guloso instancias/exemplo.txt
./programa grasp instancias/exemplo.txt 0.3 30
./programa reativo instancias/exemplo.txt 300 30
```

## Integrantes

- Vitor de Souza Reis - 202376036
- [Nome 2] - [Matrícula]
- [Nome 3] - [Matrícula]
