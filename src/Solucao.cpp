#include "../include/Solucao.h"
#include "../include/UnionFind.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

Solucao::Solucao(int n, int d) 
    : custo(0), numVertices(n), grauMaximo(d) {
    graus.resize(n, 0);
}

bool Solucao::adicionarAresta(const Aresta& a) {
    int u = a.getU();
    int v = a.getV();
    
    if (graus[u] >= grauMaximo || graus[v] >= grauMaximo) {
        return false;
    }
    
    arestas.push_back(a);
    custo += a.getPeso();
    graus[u]++;
    graus[v]++;
    return true;
}

bool Solucao::verificarValidade() {
    // Verificar número de arestas
    if (static_cast<int>(arestas.size()) != numVertices - 1) {
        valida = false;
        return false;
    }
    
    // Verificar grau máximo
    for (int i = 0; i < numVertices; i++) {
        if (graus[i] > grauMaximo) {
            valida = false;
            return false;
        }
    }
    
    // Verificar conectividade
    UnionFind uf(numVertices);
    for (const auto& a : arestas) {
        uf.unir(a.getU(), a.getV());
    }
    
    valida = (uf.getNumConjuntos() == 1);
    return valida;
}

double Solucao::getCusto() const {
    return custo;
}

int Solucao::getGrau(int v) const {
    if (v < 0 || v >= numVertices) return -1;
    return graus[v];
}

int Solucao::getNumArestas() const {
    return arestas.size();
}

bool Solucao::isValida() const {
    return valida;
}

const std::vector<Aresta>& Solucao::getArestas() const {
    return arestas;
}

int Solucao::getGrauMaximoReal() const {
    if (graus.empty()) return 0;
    return *std::max_element(graus.begin(), graus.end());
}

double Solucao::getGrauMedio() const {
    if (numVertices == 0) return 0;
    double soma = 0;
    for (int g : graus) soma += g;
    return soma / numVertices;
}

void Solucao::imprimir() const {
    std::cout << "\n========================================" << std::endl;
    std::cout << "           SOLUÇÃO DC-MST               " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Custo total: " << std::fixed << std::setprecision(2) 
              << custo << std::endl;
    std::cout << "Número de arestas: " << arestas.size() 
              << " (esperado: " << numVertices - 1 << ")" << std::endl;
    std::cout << "Grau máximo permitido: " << grauMaximo << std::endl;
    std::cout << "Grau máximo real: " << getGrauMaximoReal() << std::endl;
    std::cout << "Grau médio: " << std::fixed << std::setprecision(2) 
              << getGrauMedio() << std::endl;
    std::cout << "Válida: " << (valida ? "SIM " : "NÃO ") << std::endl;
    std::cout << "========================================\n" << std::endl;
}