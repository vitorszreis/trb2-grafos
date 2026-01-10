#ifndef GRAFO_H
#define GRAFO_H

#include "Aresta.h"
#include <vector>
#include <utility> 
#include <string>

class Grafo {
private:
    int numVertices;

    // Lista de adjacência: adj[u] = [(v1, peso1), (v2, peso2), ...]
    std::vector<std::vector<std::pair<int, double>>> adj;

public:
    // Construtor: cria grafo com n vértices
    Grafo(int n);
    
    // Adiciona aresta não-direcionada entre u e v com peso
    void adicionarAresta(int u, int v, double peso);
    
    // Retorna lista de vizinhos de u (cada vizinho é par: vértice, peso)
    const std::vector<std::pair<int, double>>& getVizinhos(int u) const;
    
    // Retorna peso da aresta entre u e v (retorna -1 se não existe)
    double getPeso(int u, int v) const;

    void imprimirFormatoGraphEditor() const;

    void salvarFormatoGraphEditor(const std::string& arquivo) const;
    
    // Retorna todas as arestas do grafo (sem duplicação)
    std::vector<Aresta> getTodasArestas() const;
    
    // Getters
    int getNumVertices() const;
    int getNumArestas() const;
    
    // Função auxiliar para debug
    void imprimirGrafo() const;
};

#endif