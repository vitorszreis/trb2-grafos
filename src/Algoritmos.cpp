#include "../include/Algoritmos.h"
#include "../include/UnionFind.h"
#include "../include/Random.h"
#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

Solucao algoritmoGuloso(const Grafo& grafo, int d) {
    int n = grafo.getNumVertices();
    
    Solucao solucao(n, d);
    UnionFind uf(n);
    
    // arestas já ordenadas
    const vector<Aresta>& arestas = grafo.getTodasArestasOrdenadas();
    
    // itera sobre arestas)
    int arestasAdicionadas = 0;
    
    for (const auto& aresta : arestas) {
        // critério de parada: se já temos n-1 arestas, a árvore está pronta
        if (arestasAdicionadas == n - 1) break;
        
        int u = aresta.getU();
        int v = aresta.getV();
        
        
        //verificação do union-find
        if (uf.find(u) == uf.find(v)) {
            continue; // se já estão no mesmo conjunto, formaria ciclo -> descarta
        }
        
        // verificação de restrição de grau (degree constraint)
        // só adicionamos a aresta se os vértices tiverem grau menor que d
        if (solucao.getGrau(u) < d && solucao.getGrau(v) < d) {
            if(solucao.adicionarAresta(aresta)) {
                // une os conjuntos no union-find para marcar que u e v estão conectados
                uf.unir(u, v);
                arestasAdicionadas++;
            }
        }
    }
    
    // verifica se a solução é válida (conexa e com n-1 arestas)
    if (!solucao.verificarValidade()) {
        cerr << "Aviso: Guloso não conseguiu conectar todo o grafo com d=" << d << endl;
    }
    
    return solucao;
}

Solucao algoritmoGulosoRandomizado(const Grafo& grafo, int d, double alfa) {
    int n = grafo.getNumVertices();
    Solucao solucao(n, d);
    UnionFind uf(n);

    // Arestas já ordenadas por peso
    const vector<Aresta>& arestas = grafo.getTodasArestasOrdenadas();
    int arestasAdicionadas = 0;

    while (arestasAdicionadas < n - 1) {
        // 1. Monta lista de candidatos viáveis
        vector<Aresta> candidatos;
        double custoMin = 1e9, custoMax = -1e9;

        for (const auto& a : arestas) {
            int u = a.getU(), v = a.getV();
            if (uf.find(u) != uf.find(v) &&
                solucao.getGrau(u) < d &&
                solucao.getGrau(v) < d) {
                candidatos.push_back(a);
                double peso = a.getPeso();
                custoMin = min(custoMin, peso);
                custoMax = max(custoMax, peso);
            }
        }

        if (candidatos.empty()) {
            cerr << "Não foi possível adicionar mais arestas sem violar o limite de grau." << endl;
            break;
        }

        // 2. Monta RCL (Lista de Candidatos Restritos)
        vector<Aresta> rcl;
        double limite = custoMin + alfa * (custoMax - custoMin);
        for (const auto& a : candidatos) {
            if (a.getPeso() <= limite) {
                rcl.push_back(a);
            }
        }

        if (rcl.empty()) {
            cerr << "RCL vazia! Ajuste o valor de alfa." << endl;
            break;
        }

        // 3. Escolhe aleatoriamente uma aresta da RCL
        int idx = randomInt(0, rcl.size() - 1);
        const Aresta& escolhida = rcl[idx];

        int u = escolhida.getU();
        int v = escolhida.getV();
        if (solucao.adicionarAresta(escolhida)) {
            uf.unir(u, v);
            arestasAdicionadas++;
        }
    }

    // Verifica se a solução é válida
    if (!solucao.verificarValidade()) {
        cerr << "Aviso: Randomizado não conseguiu conectar todo o grafo com d="
            << d << " e alfa=" << alfa << endl;
    }

    return solucao;
}