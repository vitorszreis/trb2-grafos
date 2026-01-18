#include "../include/Solucao.h"
#include "../include/UnionFind.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

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

bool Solucao::removerAresta(int u, int v) {
    for (auto it = arestas.begin(); it != arestas.end(); ++it) {
        if ((it->getU() == u && it->getV() == v) ||
            (it->getU() == v && it->getV() == u)) {
            custo -= it->getPeso();
            graus[u]--;
            graus[v]--;
            arestas.erase(it);
            return true;
        }
    }
    return false;
}

void Solucao::limpar() {
    arestas.clear();
    custo = 0;
    fill(graus.begin(), graus.end(), 0);
    valida = false;
}

void Solucao::recalcularGraus() {
    fill(graus.begin(), graus.end(), 0);
    custo = 0;
    for (const auto& a : arestas) {
        graus[a.getU()]++;
        graus[a.getV()]++;
        custo += a.getPeso();
    }
}

int Solucao::getNumVertices() const {
    return numVertices;
}

int Solucao::getGrauMaximo() const {
    return grauMaximo;
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

void Solucao::imprimir(unsigned int seed) const {
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
    std::cout << (seed != 0 ? "Semente usada: " + std::to_string(seed) : "") << std::endl;
    std::cout << "Válida: " << (valida ? "SIM " : "NÃO ") << std::endl;
    std::cout << "========================================\n" << std::endl;
}

void Solucao::salvarResultado(const string& algoritmo, const string& instancia, int d, double tempo, unsigned int seed) const {
        ofstream arquivo("../resultados/resultados.csv", ios::app);    if (arquivo.is_open()) {
        //formato: data, instancia, d, algoritmo, parametros, seed, tempo, custo
        time_t now = time(0);
        tm *ltm = localtime(&now);
        
        arquivo << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday << ",";
        arquivo << instancia << ",";
        arquivo << d << ",";
        arquivo << algoritmo << ",";
        arquivo << "N/A" << ","; // parâmetros extras (alfa, iterações)
        arquivo << seed << ",";
        arquivo << fixed << setprecision(6) << tempo << ",";
        arquivo << fixed << setprecision(2) << custo << endl;
        
        arquivo.close();

        cout << "[INFO] Resultados salvos em resultados.csv" << endl;
    } else {
        cerr << "[AVISO] Nao foi possivel gravar no arquivo CSV." << endl;
    }
}

void Solucao::imprimirFormatoGraphEditor() const {
    std::cout << "\n========================================" << std::endl;
    std::cout << "  FORMATO GRAPH EDITOR (CS ACADEMY)" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Copie e cole em: https://csacademy.com/app/graph_editor/\n" << std::endl;
    
    // Linha 1: n m
    std::cout << numVertices << " " << arestas.size() << std::endl;
    
    // Linhas seguintes: u v peso
    for (const auto& a : arestas) {
        std::cout << a.getU() << " " << a.getV() << " " 
                << static_cast<int>(a.getPeso()) << std::endl;
    }
    
    std::cout << "\n========================================\n" << std::endl;
}