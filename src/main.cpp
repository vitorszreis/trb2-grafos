#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>

#include "../include/Grafo.h"
#include "../include/Aresta.h"
#include "../include/Solucao.h"
#include "../include/UnionFind.h"
#include "../include/LeitorInstancia.h"
#include "../include/Algoritmos.h"

using namespace std;

// Declaração da função de teste (definida em Testes.cpp)
void testarGrafo();
void testarUnionFind();
void testarUnionFindComGrafo();
void testarLeituraInstancia();  
void testarFormatoGraphEditor();
void testarClasseSolucao();
void testarCacheOrdenacao();

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
        testarLeituraInstancia();
        testarFormatoGraphEditor();
        testarCacheOrdenacao();
        testarClasseSolucao();
        
        cout << "\nUso: " << argv[0] << " <executavel> <algoritmo> <parametros>\n" << endl;
        cout << "Algoritmos disponíveis:" << endl;
        cout << "  ./programa guloso <instancia> <grau>" << endl;
        cout << "  ./programa grasp <instancia> <d> <alfa> <iteracoes>" << endl;
        cout << "  ./programa reativo <instancia> <d> <iteracoes> <tamanho_bloco>\n" << endl;
        return 0;
    }

    string algoritmo = argv[1];
    Solucao solucao(0,0);
    string nomeInstancia;
    int d = 0;
    double tempoExecucao = 0.0;
    unsigned int seed = static_cast<unsigned int>(chrono::system_clock::now().time_since_epoch().count()); // semente baseada no tempo atual

    try{
        // seleção do algoritmo
        if (algoritmo == "guloso") {
            if(argc != 4) {
                cerr << "[ERRO]: Numero incorreto de parametros para o algoritmo guloso." << endl;
                cerr << "Uso: ./programa guloso <caminho_instancia> <d>" << endl;
                return 1;
            }
            cout << ">>> Executando Algoritmo Guloso..." << endl;
            nomeInstancia = argv[2];
            d = stoi(argv[3]);
            string caminhoInstancia = "../instancias/dcmst/" + nomeInstancia;

            cout << ">>> Carregando instancia: " << caminhoInstancia << " com d=" << d << "..." << endl;
            InstanciaDCMST instancia = LeitorInstancia::lerMatrizTriangular(caminhoInstancia, d);

            cout << ">>> Executando Guloso (d=" << d << ")..." << endl;
            auto inicio = chrono::high_resolution_clock::now();
            solucao = algoritmoGuloso(instancia.grafo, d);
            auto fim = chrono::high_resolution_clock::now();
            chrono::duration<double> duracao = fim - inicio;
            tempoExecucao = duracao.count();
        } 
        else if (algoritmo == "randomizado") {
            cout << "Algoritmo Randomizado ainda não implementado." << endl;
            return 0;
        }
        else if (algoritmo == "reativo") {
            cout << "Algoritmo Reativo ainda não implementado." << endl;
            return 0;
        }
        else {
            cerr << "[ERRO]: Algoritmo '" << algoritmo << "' desconhecido." << endl;
            return 1;
        }

        // exibição dos resultados
        solucao.imprimir(); 
        
        //gravação de dados para estatística (CSV)
        solucao.salvarResultado(algoritmo, nomeInstancia, d, tempoExecucao, seed);

        //formato para visualização gráfica (Graph Editor)
        solucao.imprimirFormatoGraphEditor();

    } catch (const exception& e) {
        cerr << "\nERRO CRITICO: " << e.what() << endl;
        return 1;
    }
        
    return 0;
}