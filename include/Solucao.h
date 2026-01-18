#ifndef SOLUCAO_H
#define SOLUCAO_H

#include "Aresta.h"
#include <vector>
#include <string>

class Solucao {
private:
    std::vector<Aresta> arestas;
    std::vector<int> graus;
    double custo;
    int numVertices;
    int grauMaximo;
    bool valida;

public:
    Solucao(int n, int d);
    
    // Adiciona aresta se respeitar limite de grau
    bool adicionarAresta(const Aresta& a);
    
    // Remove uma aresta da solução
    bool removerAresta(int u, int v);
    
    // Limpa a solução (remove todas as arestas)
    void limpar();
    
    // Reconstrói graus a partir das arestas (útil após modificações)
    void recalcularGraus();
    
    // Verifica validade (n-1 arestas, conectividade)
    bool verificarValidade();
    
    // Getters
    double getCusto() const;
    int getGrau(int v) const;
    int getNumArestas() const;
    int getNumVertices() const;
    int getGrauMaximo() const;
    bool isValida() const;
    const std::vector<Aresta>& getArestas() const;
    
    // Estatísticas
    int getGrauMaximoReal() const;
    double getGrauMedio() const;
    
    // Métodos de saída
    void imprimir(unsigned int seed) const;
    void imprimirFormatoGraphEditor() const;
    void salvarResultado(const std::string& nomeAlgoritmo, 
                   const std::string& nomeInstancia,
                   int d,
                   double tempoExecucao, 
                   unsigned int seed) const;

};

#endif