#include <iostream>
#include "../include/Grafo.h"
#include "../include/Aresta.h"

using namespace std;

// Declaração da função de teste (definida em Testes.cpp)
void testarGrafo();
void testarUnionFind();
void testarUnionFindComGrafo();

int main(int argc, char* argv[]) {
    cout << "========================================" << endl;
    cout << "   DC-MST - Trabalho de Grafos          " << endl;
    cout << "========================================" << endl;
    
    if (argc < 2) {
        cout << "\n[INFO] Nenhum algoritmo especificado." << endl;
        cout << "[INFO] Executando testes básicos...\n" << endl;
        
        testarGrafo();
        testarUnionFind();
        testarUnionFindComGrafo();
        
        cout << "\nUso: " << argv[0] << " <algoritmo> <parametros>\n" << endl;
        cout << "Algoritmos disponíveis:" << endl;
        cout << "  guloso <instancia> <d>" << endl;
        cout << "  grasp <instancia> <d> <alfa> <iteracoes>" << endl;
        cout << "  reativo <instancia> <d> <iteracoes> <tamanho_bloco>\n" << endl;
        return 0;
    }
        
    return 0;
}