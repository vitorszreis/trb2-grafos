#ifndef LEITOR_INSTANCIA_H
#define LEITOR_INSTANCIA_H

#include "Grafo.h"
#include <string>

// Estrutura que representa uma instância DC-MST completa
struct InstanciaDCMST {
    Grafo grafo;
    int grauMaximo;
    std::string nomeInstancia;
    int numVertices;
    
    InstanciaDCMST(int n, int d, const std::string& nome) 
        : grafo(n), grauMaximo(d), nomeInstancia(nome), numVertices(n) {}
};

class LeitorInstancia {
public:
    // Lê instância no formato matriz triangular (TSPLIB)
    static InstanciaDCMST lerMatrizTriangular(
        const std::string& caminhoArquivo, 
        int grauMaximo
    );
    
    // Retorna solução ótima conhecida (lê de bestSolutions.txt)
    static int obterSolucaoOtima(
        const std::string& nomeInstancia, 
        int grauMaximo
    );
    
private:
    // Extrai nome do arquivo do caminho completo
    static std::string extrairNomeArquivo(const std::string& caminho);
};

#endif