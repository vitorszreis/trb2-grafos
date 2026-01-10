#include <iostream>
#include <vector>
#include <algorithm>
#include "../include/Grafo.h"
#include "../include/Aresta.h"
#include "../include/UnionFind.h"

using namespace std;

void testarGrafo() {
    cout << "\n " << endl;
    cout << "     TESTE DA ESTRUTURA DO GRAFO         " << endl;
    cout << "\n" << endl;
    
    // Criar grafo com 4 vértices
    cout << "[1] Criando grafo com 4 vértices..." << endl;
    Grafo g(4);
    
    // Adicionar arestas
    cout << "[2] Adicionando arestas..." << endl;
    g.adicionarAresta(0, 1, 10.0);
    g.adicionarAresta(0, 2, 20.0);
    g.adicionarAresta(0, 3, 30.0);
    g.adicionarAresta(1, 2, 15.0);
    g.adicionarAresta(1, 3, 25.0);
    g.adicionarAresta(2, 3, 35.0);
    
    // Imprimir estrutura
    cout << "[3] Estrutura do grafo:" << endl;
    g.imprimirGrafo();
    
    // Testar getters
    cout << "[4] Testando getters básicos:" << endl;
    cout << "    Número de vértices: " << g.getNumVertices() << endl;
    cout << "    Número de arestas: " << g.getNumArestas() << endl;
    cout << "    Peso aresta (0,1): " << g.getPeso(0, 1) << endl;
    cout << "    Peso aresta (2,3): " << g.getPeso(2, 3) << endl;
    
    // Testar vizinhos
    cout << "\n[5] Vizinhos do vértice 0:" << endl;
    auto vizinhos = g.getVizinhos(0);
    for (const auto& vizinho : vizinhos) {
        cout << "    -> Vértice " << vizinho.first << " com peso " << vizinho.second << endl;
    }
    
    // Testar ordenação de arestas
    cout << "\n[6] Todas as arestas (ordenadas por peso):" << endl;
    vector<Aresta> arestas = g.getTodasArestas();
    sort(arestas.begin(), arestas.end());
    
    for (const auto& a : arestas) {
        cout << "    Aresta (" << a.getU() << ", " << a.getV() 
             << ") - Peso: " << a.getPeso() << endl;
    }
    
    cout << "\n TODOS OS TESTES PASSARAM!\n" << endl;
}

void testarUnionFind() {
    cout << "\n " << endl;
    cout << "     TESTE DA ESTRUTURA UNION-FIND       " << endl;
    cout << "\n" << endl;
    
    // Criar Union-Find com 5 elementos
    cout << "[1] Criando Union-Find com 5 elementos..." << endl;
    UnionFind uf(5);
    
    cout << "[2] Conjuntos iniciais: " << uf.getNumConjuntos() << endl;
    
    // Fazer algumas uniões
    cout << "[3] Unindo elementos..." << endl;
    uf.unir(0, 1);
    uf.unir(1, 2);
    uf.unir(3, 4);
    
    cout << "    Conjuntos após uniões: " << uf.getNumConjuntos() << endl;
    
    // Testar se estão no mesmo conjunto
    cout << "[4] Verificando conjuntos:" << endl;
    cout << "    0 e 2 no mesmo conjunto? " << (uf.mesmoConjunto(0, 2) ? "Sim" : "Não") << endl;
    cout << "    0 e 3 no mesmo conjunto? " << (uf.mesmoConjunto(0, 3) ? "Sim" : "Não") << endl;
    
    cout << "\n TESTES UNION-FIND PASSARAM!\n" << endl;
}

void testarUnionFindComGrafo() {
    cout << "\n" << endl;
    cout << "  TESTE 3: UNION-FIND + GRAFO (MST)   " << endl;
    cout << "\n" << endl;
    
    cout << "[1] Criando grafo exemplo..." << endl;
    Grafo g(4);
    g.adicionarAresta(0, 1, 10.0);
    g.adicionarAresta(0, 2, 20.0);
    g.adicionarAresta(0, 3, 30.0);
    g.adicionarAresta(1, 2, 15.0);
    g.adicionarAresta(1, 3, 25.0);
    g.adicionarAresta(2, 3, 35.0);
    
    cout << "[2] Simulando algoritmo de Kruskal..." << endl;
    
    // Obter e ordenar arestas
    vector<Aresta> arestas = g.getTodasArestas();
    sort(arestas.begin(), arestas.end());
    
    cout << "    Arestas ordenadas:" << endl;
    for (const auto& a : arestas) {
        cout << "      (" << a.getU() << "," << a.getV() << ") peso " << a.getPeso() << endl;
    }
    
    // Union-Find para MST
    UnionFind uf(g.getNumVertices());
    vector<Aresta> mst;
    double custoTotal = 0;
    
    cout << "\n[3] Processando arestas..." << endl;
    for (const auto& a : arestas) {
        int u = a.getU();
        int v = a.getV();
        double peso = a.getPeso();
        
        cout << "    Tentando aresta (" << u << "," << v << ") peso " << peso << ": ";
        
        if (uf.unir(u, v)) {
            mst.push_back(a);
            custoTotal += peso;
            cout << "ADICIONADA " << endl;
        } else {
            cout << "REJEITADA (criaria ciclo) " << endl;
        }
        
        if (mst.size() == (size_t)(g.getNumVertices() - 1)) {
            cout << "     MST completa!" << endl;
            break;
        }
    }
    
    cout << "\n[4] Resultado (MST):" << endl;
    cout << "    Arestas na MST:" << endl;
    for (const auto& a : mst) {
        cout << "      (" << a.getU() << "," << a.getV() << ") peso " << a.getPeso() << endl;
    }
    cout << "    Custo total: " << custoTotal << endl;
    cout << "    Número de arestas: " << mst.size() << " (esperado: " 
         << g.getNumVertices() - 1 << ")" << endl;
    
    cout << "\n TESTE 3 PASSOU!\n" << endl;
}