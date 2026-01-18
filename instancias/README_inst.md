# Formato das Instâncias DC-MST

## Origem dos Dados

Dataset de **Degree Constrained Minimum Spanning Tree (DC-MST)** obtido do repositório Mathprog/OR-Library de Andreas Ernst.**README**

Referência principal: Ernst, Krishnamoorthy & Sharaiha, “Comparisons of algorithms for the degree constrained minimum spanning tree”.**README**

As famílias de instâncias foram geradas por:

* CRD e SYM: A. Volgenant.**README**
* Demais (SHRD e STR): A. Ernst e M. Krishnamoorthy.**README**

## Descrição do Problema

O problema DC-MST consiste em encontrar uma árvore geradora de custo mínimo em um grafo completo, sujeita à restrição de que o grau de cada vértice seja no máximo  **d** .**README**

## Tipos de Instâncias

As instâncias são divididas em quatro famílias, todas armazenadas como **matriz de custos** (não há lista explícita de arestas).**README**

## CRD (Coordenadas Euclidianas)

* Derivadas de pontos em um plano euclidiano 2D com distâncias truncadas para inteiro.**README**
* Nome típico: `crd100`, `crd300`, `crd500`, `crd700` etc.**README**
* Apesar do nome sugerir coordenadas, os arquivos distribuídos aqui já contêm **1/2 da matriz de custos simétrica** entre todos os pares de vértices.**README**

## SYM (Simétricas)

* Custos gerados diretamente como matriz simétrica, sem interpretação geométrica específica.**README**
* Exemplos: `sym300`, `sym500`, `sym700` (várias instâncias numeradas de 0 a 9).**bestSolutions.txt**+1

## SHRD (Shortest Paths)

* Baseadas em grafos derivados de problemas de caminhos mínimos, resultando em estruturas menos “aleatórias” que SYM.**README**
* Exemplos de nomes: `shrd259`, `shrd300`, `shrd400`, `shrd500`, `shrd700`.**bestSolutions.txt**

## STR (Estruturadas)

* Grafos com estrutura especial (por exemplo, blocos ou padrões regulares) usados para testar desempenho em topologias específicas.**README**
* Exemplos: `str300`, `str400`, `str500`, `str700`, com instâncias numeradas (`str507`, `str708` etc.).**bestSolutions.txt**

## Formato dos Arquivos de Instância

Todos os arquivos, exceto os CRD originais, são distribuídos aqui como **metade da matriz de custos simétrica** entre todos os pares de nós.**README**

## Matriz Triangular Inferior

A leitura deve seguir o seguinte esquema (matriz triangular inferior, sem diagonal):**README**

* O arquivo contém n(n−1)2\frac{n(n-1)}{2}**2**n**(**n**−**1**)** números inteiros, onde **n** é o número de vértices.**README**
* Os valores representam os custos cijc_{ij}**c**ij para cada par (i,j)(i,j)**(**i**,**j**)** com i>ji > j**i**>**j**.**README**
* A ordem é:

<pre class="not-prose w-full rounded font-mono text-sm font-extralight"><div class="codeWrapper text-light selection:text-super selection:bg-super/10 my-md relative flex flex-col rounded-lg font-mono text-sm font-normal bg-subtler"><div class="translate-y-xs -translate-x-xs bottom-xl mb-xl flex h-0 items-start justify-end sm:sticky sm:top-xs"><div class="overflow-hidden rounded-full border-subtlest ring-subtlest divide-subtlest bg-base"><div class="border-subtlest ring-subtlest divide-subtlest bg-subtler"><button data-testid="copy-code-button" aria-label="Copy code" type="button" class="focus-visible:bg-subtle hover:bg-subtle text-quiet  hover:text-foreground dark:hover:bg-subtle font-sans focus:outline-none outline-none outline-transparent transition duration-300 ease-out select-none items-center relative group/button font-semimedium justify-center text-center items-center rounded-full cursor-pointer active:scale-[0.97] active:duration-150 active:ease-outExpo origin-center whitespace-nowrap inline-flex text-sm h-8 aspect-square" data-state="closed"><div class="flex items-center min-w-0 gap-two justify-center"><div class="flex shrink-0 items-center justify-center size-4"><svg class="inline-flex fill-current" width="16" height="16"><use xlink:href="#pplx-icon-copy"></use></svg></div></div></button></div></div></div><div class="-mt-xl"><div><div data-testid="code-language-indicator" class="text-quiet bg-subtle py-xs px-sm inline-block rounded-br rounded-tl-lg text-xs font-thin">text</div></div><div><span><code><span><span>d[2,1]
</span></span><span>d[3,1] d[3,2]
</span><span>d[4,1] d[4,2] d[4,3]
</span><span>...
</span><span>d[n,1] d[n,2] ... d[n,n-1]
</span><span></span></code></span></div></div></div></pre>

* Em pseudocódigo, a leitura é feita com dois laços aninhados:**README**

<pre class="not-prose w-full rounded font-mono text-sm font-extralight"><div class="codeWrapper text-light selection:text-super selection:bg-super/10 my-md relative flex flex-col rounded-lg font-mono text-sm font-normal bg-subtler"><div class="translate-y-xs -translate-x-xs bottom-xl mb-xl flex h-0 items-start justify-end sm:sticky sm:top-xs"><div class="overflow-hidden rounded-full border-subtlest ring-subtlest divide-subtlest bg-base"><div class="border-subtlest ring-subtlest divide-subtlest bg-subtler"><button data-testid="copy-code-button" aria-label="Copy code" type="button" class="focus-visible:bg-subtle hover:bg-subtle text-quiet  hover:text-foreground dark:hover:bg-subtle font-sans focus:outline-none outline-none outline-transparent transition duration-300 ease-out select-none items-center relative group/button font-semimedium justify-center text-center items-center rounded-full cursor-pointer active:scale-[0.97] active:duration-150 active:ease-outExpo origin-center whitespace-nowrap inline-flex text-sm h-8 aspect-square" data-state="closed"><div class="flex items-center min-w-0 gap-two justify-center"><div class="flex shrink-0 items-center justify-center size-4"><svg class="inline-flex fill-current" width="16" height="16"><use xlink:href="#pplx-icon-copy"></use></svg></div></div></button></div></div></div><div class="-mt-xl"><div><div data-testid="code-language-indicator" class="text-quiet bg-subtle py-xs px-sm inline-block rounded-br rounded-tl-lg text-xs font-thin">text</div></div><div><span><code><span><span>FOR i = 1 .. n:
</span></span><span>  FOR j = 1 .. i-1:
</span><span>    distance(i,j) = distance(j,i) = próximo_valor_lido
</span><span></span></code></span></div></div></div></pre>

Na prática, isso significa que:

* Há  **n−1 linhas lógicas** , onde a “linha” ii**i** (para i≥2i \ge 2**i**≥**2**) possui i−1i-1**i**−**1** valores.
* Os valores podem estar quebrados em várias linhas físicas de texto, mas a ordem é exatamente essa sequência triangular.

## Número de Vértices (n)

* O valor de **n** é codificado no nome da instância.**README**
* Para a maioria dos conjuntos, os **dois primeiros dígitos** do nome indicam nn**n**, e o último dígito distingue instâncias.**README**
* Exceção: instâncias de 100 nós usam `10x` (por exemplo, `crd100` corresponde a 100 nós).

## Grafo Completo

* Todas as instâncias representam  **grafos completos** , isto é, existe uma aresta para cada par de vértices i≠ji \neq j**i****=**j.**README**
* Logo, o número de arestas é m=n(n−1)2m = \frac{n(n-1)}{2}**m**=**2**n**(**n**−**1**)**.**README**

## Parâmetros do Problema

Para cada instância, o valor de **d** (grau máximo) não fica no arquivo de dados, e sim é definido externamente na experimentação.**README**

No arquivo de melhores soluções, cada linha associa uma instância a um valor de **d** e ao melhor custo conhecido.**bestSolutions.txt**

## Arquivo de Melhores Soluções (`bestSolutions.txt`)

O arquivo `bestSolutions.txt` contém as **melhores soluções conhecidas** (em 2001) para cada instância e vários valores de  **d** .**bestSolutions.txt**+1

## Formato de Cada Linha

<pre class="not-prose w-full rounded font-mono text-sm font-extralight"><div class="codeWrapper text-light selection:text-super selection:bg-super/10 my-md relative flex flex-col rounded-lg font-mono text-sm font-normal bg-subtler"><div class="translate-y-xs -translate-x-xs bottom-xl mb-xl flex h-0 items-start justify-end sm:sticky sm:top-xs"><div class="overflow-hidden rounded-full border-subtlest ring-subtlest divide-subtlest bg-base"><div class="border-subtlest ring-subtlest divide-subtlest bg-subtler"><button data-testid="copy-code-button" aria-label="Copy code" type="button" class="focus-visible:bg-subtle hover:bg-subtle text-quiet  hover:text-foreground dark:hover:bg-subtle font-sans focus:outline-none outline-none outline-transparent transition duration-300 ease-out select-none items-center relative group/button font-semimedium justify-center text-center items-center rounded-full cursor-pointer active:scale-[0.97] active:duration-150 active:ease-outExpo origin-center whitespace-nowrap inline-flex text-sm h-8 aspect-square" data-state="closed"><div class="flex items-center min-w-0 gap-two justify-center"><div class="flex shrink-0 items-center justify-center size-4"><svg class="inline-flex fill-current" width="16" height="16"><use xlink:href="#pplx-icon-copy"></use></svg></div></div></button></div></div></div><div class="-mt-xl"><div><div data-testid="code-language-indicator" class="text-quiet bg-subtle py-xs px-sm inline-block rounded-br rounded-tl-lg text-xs font-thin">text</div></div><div><span><code><span><span>nome_instancia   d   custo   status [informação_extra]
</span></span><span></span></code></span></div></div></div></pre>

* `nome_instancia`: por exemplo, `crd100`, `sym300`, `str507`.**bestSolutions.txt**
* `d`: grau máximo considerado (tipicamente 2, 3, 4 ou 5).**bestSolutions.txt**
* `custo`: valor da árvore geradora que respeita o grau máximo d.**bestSolutions.txt**
* `status`:
  * `*` indica solução ótima provada.**bestSolutions.txt**
  * `H` indica solução obtida por heurística; às vezes acompanha `LB=...` com o lower bound.**bestSolutions.txt**

Exemplo real:**bestSolutions.txt**

<pre class="not-prose w-full rounded font-mono text-sm font-extralight"><div class="codeWrapper text-light selection:text-super selection:bg-super/10 my-md relative flex flex-col rounded-lg font-mono text-sm font-normal bg-subtler"><div class="translate-y-xs -translate-x-xs bottom-xl mb-xl flex h-0 items-start justify-end sm:sticky sm:top-xs"><div class="overflow-hidden rounded-full border-subtlest ring-subtlest divide-subtlest bg-base"><div class="border-subtlest ring-subtlest divide-subtlest bg-subtler"><button data-testid="copy-code-button" aria-label="Copy code" type="button" class="focus-visible:bg-subtle hover:bg-subtle text-quiet  hover:text-foreground dark:hover:bg-subtle font-sans focus:outline-none outline-none outline-transparent transition duration-300 ease-out select-none items-center relative group/button font-semimedium justify-center text-center items-center rounded-full cursor-pointer active:scale-[0.97] active:duration-150 active:ease-outExpo origin-center whitespace-nowrap inline-flex text-sm h-8 aspect-square" data-state="closed"><div class="flex items-center min-w-0 gap-two justify-center"><div class="flex shrink-0 items-center justify-center size-4"><svg class="inline-flex fill-current" width="16" height="16"><use xlink:href="#pplx-icon-copy"></use></svg></div></div></button></div></div></div><div class="-mt-xl"><div><div data-testid="code-language-indicator" class="text-quiet bg-subtle py-xs px-sm inline-block rounded-br rounded-tl-lg text-xs font-thin">text</div></div><div><span><code><span><span>str507  3    8788 *
</span></span><span>sym300  5     959 *
</span><span>sym701  2    1931 H  LB=1919.45
</span><span></span></code></span></div></div></div></pre>

## Conversão para o Formato do Programa

Seu programa espera instâncias no seguinte formato:

<pre class="not-prose w-full rounded font-mono text-sm font-extralight"><div class="codeWrapper text-light selection:text-super selection:bg-super/10 my-md relative flex flex-col rounded-lg font-mono text-sm font-normal bg-subtler"><div class="translate-y-xs -translate-x-xs bottom-xl mb-xl flex h-0 items-start justify-end sm:sticky sm:top-xs"><div class="overflow-hidden rounded-full border-subtlest ring-subtlest divide-subtlest bg-base"><div class="border-subtlest ring-subtlest divide-subtlest bg-subtler"><button data-testid="copy-code-button" aria-label="Copy code" type="button" class="focus-visible:bg-subtle hover:bg-subtle text-quiet  hover:text-foreground dark:hover:bg-subtle font-sans focus:outline-none outline-none outline-transparent transition duration-300 ease-out select-none items-center relative group/button font-semimedium justify-center text-center items-center rounded-full cursor-pointer active:scale-[0.97] active:duration-150 active:ease-outExpo origin-center whitespace-nowrap inline-flex text-sm h-8 aspect-square" data-state="closed"><div class="flex items-center min-w-0 gap-two justify-center"><div class="flex shrink-0 items-center justify-center size-4"><svg class="inline-flex fill-current" width="16" height="16"><use xlink:href="#pplx-icon-copy"></use></svg></div></div></button></div></div></div><div class="-mt-xl"><div><div data-testid="code-language-indicator" class="text-quiet bg-subtle py-xs px-sm inline-block rounded-br rounded-tl-lg text-xs font-thin">text</div></div><div><span><code><span><span>n
</span></span><span>m
</span><span>d
</span><span>u v peso
</span><span>u v peso
</span><span>...
</span><span></span></code></span></div></div></div></pre>

Onde:

* `n`: número de vértices (inferido do nome do arquivo).**README**
* `m`: número de arestas; como o grafo é completo, m=n(n−1)2m = \frac{n(n-1)}{2}**m**=**2**n**(**n**−**1**)**.**README**
* `d`: grau máximo escolhido para o experimento (por exemplo, 2, 3, 4 ou 5).**bestSolutions.txt**
* `u v peso`: uma linha por aresta, com os vértices `u`, `v` e o custo correspondente.**README**

## Passos de Conversão

1. **Determinar n**
   * Usar a convenção do nome (por exemplo, `crd100` → n = 100, `sym300` → n = 300).**README**
2. **Ler todos os valores de custo** como uma sequência linear seguindo a ordem triangular inferior.**README**
3. **Construir a matriz completa**
   * Preencher `distance[i][j]` e `distance[j][i]` com cada valor lido para i>ji > j**i**>**j**.**README**
4. **Gerar lista de arestas**
   * Para todo par 0≤i<j<n0 \le i < j < n**0**≤**i**<**j**<**n**, criar uma aresta `(i, j, distance[i][j])`.**README**
5. **Calcular m**
   * m=n(n−1)2m = \frac{n(n-1)}{2}**m**=**2**n**(**n**−**1**)**.**README**
6. **Escolher d**
   * Definir o grau máximo conforme o cenário de teste (por exemplo, d = 2, 3, 4 ou 5).**bestSolutions.txt**
7. **Escrever arquivo de saída** no formato:

<pre class="not-prose w-full rounded font-mono text-sm font-extralight"><div class="codeWrapper text-light selection:text-super selection:bg-super/10 my-md relative flex flex-col rounded-lg font-mono text-sm font-normal bg-subtler"><div class="translate-y-xs -translate-x-xs bottom-xl mb-xl flex h-0 items-start justify-end sm:sticky sm:top-xs"><div class="overflow-hidden rounded-full border-subtlest ring-subtlest divide-subtlest bg-base"><div class="border-subtlest ring-subtlest divide-subtlest bg-subtler"><button data-testid="copy-code-button" aria-label="Copy code" type="button" class="focus-visible:bg-subtle hover:bg-subtle text-quiet  hover:text-foreground dark:hover:bg-subtle font-sans focus:outline-none outline-none outline-transparent transition duration-300 ease-out select-none items-center relative group/button font-semimedium justify-center text-center items-center rounded-full cursor-pointer active:scale-[0.97] active:duration-150 active:ease-outExpo origin-center whitespace-nowrap inline-flex text-sm h-8 aspect-square" data-state="closed"><div class="flex items-center min-w-0 gap-two justify-center"><div class="flex shrink-0 items-center justify-center size-4"><svg class="inline-flex fill-current" width="16" height="16"><use xlink:href="#pplx-icon-copy"></use></svg></div></div></button></div></div></div><div class="-mt-xl"><div><div data-testid="code-language-indicator" class="text-quiet bg-subtle py-xs px-sm inline-block rounded-br rounded-tl-lg text-xs font-thin">text</div></div><div><span><code><span><span>n
</span></span><span>m
</span><span>d
</span><span>0 1 peso01
</span><span>0 2 peso02
</span><span>...
</span><span>i j pesoij
</span><span></span></code></span></div></div></div></pre>

## Uso Típico no Projeto

* Usar as instâncias CRD, SYM, SHRD e STR como benchmarks para o algoritmo de DC-MST.**README**
* Converter cada arquivo original para o formato de entrada do programa antes de rodar os experimentos.**README**
* Validar os resultados comparando o custo obtido com os valores de `bestSolutions.txt` para o mesmo par `(instância, d)`.**bestSolutions.txt**

1. [https://ppl-ai-file-upload.s3.amazonaws.com/web/direct-files/attachments/images/42432292/6dc8b931-4f45-4380-9fd7-95ddcf53b277/image.jpg](https://ppl-ai-file-upload.s3.amazonaws.com/web/direct-files/attachments/images/42432292/6dc8b931-4f45-4380-9fd7-95ddcf53b277/image.jpg)
2. [https://ppl-ai-file-upload.s3.amazonaws.com/web/direct-files/attachments/42432292/e8b79bb2-cac5-47a7-8854-2737d40be823/bestSolutions.txt](https://ppl-ai-file-upload.s3.amazonaws.com/web/direct-files/attachments/42432292/e8b79bb2-cac5-47a7-8854-2737d40be823/bestSolutions.txt)
3. [https://ppl-ai-file-upload.s3.amazonaws.com/web/direct-files/attachments/42432292/1db46077-b158-48ef-bc9c-960654825b5f/README](https://ppl-ai-file-upload.s3.amazonaws.com/web/direct-files/attachments/42432292/1db46077-b158-48ef-bc9c-960654825b5f/README)
