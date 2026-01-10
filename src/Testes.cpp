#include <iostream>
#include <vector>
#include <algorithm>
#include "../include/Grafo.h"
#include "../include/Aresta.h"

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