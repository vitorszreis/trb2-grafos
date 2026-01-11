INSTRUÇÕES DE COMPILAÇÃO E EXECUÇÃO
=========================================

## COMPILAÇÃO

### Primeira compilação

```
mkdir build
cd build
cmake ..
make

./programa

```


Para próximas compilacoes

make clean && make

./programa

Isso irá compilar todos os arquivos .cpp e gerar o executável 'programa'.


## LIMPEZA

Para remover arquivos compilados:

    make clean

## EXECUÇÃO

### Algoritmo Guloso

    ./programa guloso instancias/exemplo.txt

### Algoritmo GRASP

    ./programa grasp instancias/exemplo.txt`<alfa>` <num_iteracoes>

    Exemplo:
    ./programa grasp instancias/exemplo.txt 0.3 30

### Algoritmo Reativo

    ./programa reativo instancias/exemplo.txt <num_iteracoes> <tamanho_bloco>

    Exemplo:
    ./programa reativo instancias/exemplo.txt 300 30

### Definir Semente Manualmente

    ./programa --seed=12345 guloso instancias/exemplo.txt

## FORMATO DAS INSTÂNCIAS

As instâncias devem seguir o formato DC-MST padrão:

- Linha 1: número de vértices
- Linha 2: número de arestas
- Linha 3: grau máximo d
- Linhas seguintes: u v peso (uma aresta por linha)

## SAÍDA

- Solução impressa no console
- Arquivo de resultado em resultados/solucao.txt
- Registro CSV em resultados/resultados.csv
