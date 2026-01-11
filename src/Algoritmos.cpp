#include "../include/Algoritmos.h"
#include "../include/UnionFind.h"
#include <iostream>
#include <vector>

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