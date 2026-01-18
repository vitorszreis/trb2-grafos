#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>

#include "../include/Grafo.h"
#include "../include/Aresta.h"
#include "../include/Solucao.h"
#include "../include/UnionFind.h"
#include "../include/LeitorInstancia.h"
#include "../include/Algoritmos.h"
#include "../include/Random.h"

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

    setRandomSeed(time(0)); // inicializa o gerador de números aleatórios

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
            if(argc != 5) {
                cerr << "[ERRO]: Numero incorreto de parametros para o algoritmo randomizado." << endl;
                cerr << "Uso: ./programa randomizado <caminho_instancia> <d> <alfa>" << endl;
                return 1;
            }
            nomeInstancia = argv[2];
            d = stoi(argv[3]);
            double alfa = stod(argv[4]);
            string caminhoInstancia = "../instancias/dcmst/" + nomeInstancia;
        
            InstanciaDCMST instancia = LeitorInstancia::lerMatrizTriangular(caminhoInstancia, d);
        
            auto inicio = chrono::high_resolution_clock::now();
            solucao = algoritmoGulosoRandomizado(instancia.grafo, d, alfa);
            auto fim = chrono::high_resolution_clock::now();
            chrono::duration<double> duracao = fim - inicio;
            tempoExecucao = duracao.count();
        }
 // Algoritmo reativo
else if (algoritmo == "reativo") {
    if(argc != 6){
        cerr << "[Erro]: Número incorreto de parametros para o algoritmo reativo." << endl;
        cerr << "Uso: ./programa reativo <caminho_instancia> <d> <iteracoes> <tamanho_bloco>" << endl;  
        cerr << "Exemplo: ./programa reativo crd100 3 100 10" << endl;  
        return 1;
    }
    
    cout << ">>> Executando Algoritmo Guloso Randomizado Reativo..." << endl;
    nomeInstancia = argv[2];
    d = stoi(argv[3]);
    int maxIteracoes = stoi(argv[4]);
    int tamanhoBloco = stoi(argv[5]);

    if(maxIteracoes < 300){
        cout << "[Aviso] Especificação do trabalho é de pelo menos 300 iterações" << endl;
        cout << "        Está sendo usado " << maxIteracoes << " iterações" << endl;
    }
    
    if(tamanhoBloco < 30 || tamanhoBloco > 50){
        cout << "[Aviso] A especificação sugere blocos de 30 a 50 iterações" << endl;
        cout << "        Está sendo usado blocos de " << tamanhoBloco << " iterações" << endl;
    }
    
    string caminhoInstancia = "../instancias/dcmst/" + nomeInstancia;
    cout << ">>> Carregando instancia: " << caminhoInstancia << " com d=" << d << endl;
    InstanciaDCMST instancia = LeitorInstancia::lerMatrizTriangular(caminhoInstancia, d);

    vector<double> alphas = {0.1, 0.3, 0.5};

    cout << "\n>>> Parâmetros do algoritmo reativo" << endl;  
    cout << "    Valores de alpha: ";
    for(double a: alphas) cout << a << " ";
    cout << endl;
    cout << "    Iterações totais: " << maxIteracoes << endl;
    cout << "    Tamanho do bloco: " << tamanhoBloco << endl;
    cout << "    Semente: " << seed << endl;
    cout << "========================================\n" << endl;
    
    auto inicio = chrono::high_resolution_clock::now();
    
    // Executa o algoritmo reativo
    solucao = algoritmoGulosoRandomizadoReativo(
        instancia.grafo, 
        d, 
        alphas,
        tamanhoBloco,
        maxIteracoes   
    );
    
    auto fim = chrono::high_resolution_clock::now();
    chrono::duration<double> duracao = fim - inicio;
    tempoExecucao = duracao.count();
    
    // Criando uma string com os parâmetros para o .csv
    string parametrosExtra = "alphas=[";
    for (size_t i = 0; i < alphas.size(); i++) {
        parametrosExtra += to_string(alphas[i]);
        if (i < alphas.size() - 1) parametrosExtra += ",";
    }
    parametrosExtra += "]|bloco=" + to_string(tamanhoBloco) + 
                      "|iter=" + to_string(maxIteracoes);
    
    // Salva resultado do reativo
    solucao.salvarResultado("reativo", nomeInstancia, d, tempoExecucao, seed);
    
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