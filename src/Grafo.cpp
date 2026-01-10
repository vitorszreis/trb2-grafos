#include "../include/Grafo.h"
#include <iostream>
#include <iomanip>

// Construtor
Grafo::Grafo(int n) : numVertices(n) {
    adj.resize(n);
}

// Adiciona aresta NÃO-DIRECIONADA
void Grafo::adicionarAresta(int u, int v, double peso) {
    // Validação básica
    if (u < 0 || u >= numVertices || v < 0 || v >= numVertices) {
        std::cerr << "[ERRO] Vértices inválidos: " << u << ", " << v << std::endl;
        return;
    }
    
    if (u == v) {
        std::cerr << "[AVISO] Tentativa de adicionar auto-loop em " << u << std::endl;
        return;
    }
    
    // Adicionar nos dois sentidos (grafo não-direcionado)
    adj[u].push_back({v, peso});
    adj[v].push_back({u, peso});
}

// Retorna vizinhos de u
const std::vector<std::pair<int, double>>& Grafo::getVizinhos(int u) const {
    if (u < 0 || u >= numVertices) {
        std::cerr << "[ERRO] Vértice inválido: " << u << std::endl;
        static std::vector<std::pair<int, double>> vazio;
        return vazio;
    }
    return adj[u];
}

// Retorna peso da aresta entre u e v
double Grafo::getPeso(int u, int v) const {
    if (u < 0 || u >= numVertices || v < 0 || v >= numVertices) {
        return -1;
    }
    
    // Buscar v na lista de adjacência de u
    for (auto& par : adj[u]) {  
        int vizinho = par.first;
        double peso = par.second;
        
        if (vizinho == v) {
            return peso;
        }
    }
    
    return -1;  // Aresta não existe
}

// Retorna todas as arestas (sem duplicação)
std::vector<Aresta> Grafo::getTodasArestas() const {
    std::vector<Aresta> arestas;
    
    // Percorrer todos os vértices
    for (int u = 0; u < numVertices; u++) {
        // Para cada vizinho de u
        for (auto& par : adj[u]) {  // ✅ CORRIGIDO
            int v = par.first;
            double peso = par.second;
            
            // Adicionar apenas se u < v (evita duplicação)
            if (u < v) {
                arestas.push_back(Aresta(u, v, peso));
            }
        }
    }
    
    return arestas;
}

// Retorna número de vértices
int Grafo::getNumVertices() const {
    return numVertices;
}

// Retorna número de arestas
int Grafo::getNumArestas() const {
    int count = 0;
    for (int u = 0; u < numVertices; u++) {
        count += adj[u].size();
    }
    // Dividir por 2 pois grafo é não-direcionado (cada aresta contada 2x)
    return count / 2;
}

// Imprime grafo (para debug)
void Grafo::imprimirGrafo() const {
    std::cout << "\n========================================" << std::endl;
    std::cout << "         ESTRUTURA DO GRAFO             " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Vértices: " << numVertices << std::endl;
    std::cout << "Arestas: " << getNumArestas() << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    for (int u = 0; u < numVertices; u++) {
        std::cout << "Vértice " << std::setw(3) << u << " -> ";
        
        if (adj[u].empty()) {
            std::cout << "(sem vizinhos)";
        } else {
            std::cout << "{ ";
            for (size_t i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i].first;          
                double peso = adj[u][i].second;   
                
                std::cout << "(" << v << ", " << std::fixed 
                         << std::setprecision(1) << peso << ")";
                if (i < adj[u].size() - 1) std::cout << ", ";
            }
            std::cout << " }";
        }
        std::cout << std::endl;
    }
    
}