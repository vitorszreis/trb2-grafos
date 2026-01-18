#include "../include/Experimentos.h"
#include "../include/Algoritmos.h"
#include "../include/LeitorInstancia.h"
#include "../include/Solucao.h"
#include "../include/Random.h" 

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <numeric>

using namespace std;
namespace fs = std::filesystem;

void executarExperimentos() {
    // --- CONFIGURAÇÕES DO EXPERIMENTO (Conforme Enunciado) ---
    string arquivoSaida = "../resultados/tabela_resultados_final.csv";
    
    // Parâmetros Gerais
    //Instancias
    vector<string> instancias = {"crd501", "sym302", "shrd159", "shrd300","str700"};
    // O enunciado não especifica os graus, mas para comparar com a literatura (Ernst),
    // é ideal rodar com d variando. Se quiser rodar apenas o padrão, deixe apenas {5}.
    vector<int> grausTeste = {2, 3, 4, 5}; 
    
    // "Os algoritmos devem ser executados 10 vezes para cada instância"
    int numRepsEstatisticas = 10;       

    // Parâmetros Randomizado
    // "O algoritmo construtivo deve ser chamado pelo menos 30 vezes"
    vector<double> alphas = {0.1, 0.3, 0.5}; // "Cada grupo pode escolher três valores de alfa"
    
    unsigned int seed = static_cast<unsigned int>(chrono::system_clock::now().time_since_epoch().count()); // semente baseada no tempo atual
    setRandomSeed(seed); // inicializa o gerador de números aleatórios
    
    // Parâmetros Reativo
    // "Construtivo chamado pelo menos 300 vezes, com blocos de 30 a 50"
    int reatIter = 300;
    int reatBloco = 30; 
    vector<double> alphasReativo = {0.1, 0.3, 0.5};

    // --- PREPARAÇÃO DO ARQUIVO CSV ---
    ofstream csv(arquivoSaida);
    if (!csv.is_open()) {
        cerr << "ERRO CRÍTICO: Não foi possível criar o arquivo " << arquivoSaida << endl;
        return;
    }

    // Cabeçalho alinhado com o que será pedido nas tabelas do relatório:
    // Instância | Grau | Guloso | Rand (Min/Med/Tempo) | Reativo (Min/Med/Tempo)
    csv << "Instancia;Grau;Guloso_Custo;Guloso_Tempo;";
    for(double a : alphas) 
        csv << "Rand_" << a << "_Melhor;" << "Rand_" << a << "_Media;" << "Rand_" << a << "_Tempo_Medio;";
    csv << "Reativo_Melhor;Reativo_Media;Reativo_Tempo_Medio\n";

    cout << "===========================================" << endl;
    cout << "   MÓDULO DE EXPERIMENTOS (ENUNCIADO)      " << endl;
    cout << "===========================================" << endl;
    cout << "Reps Estatísticas (Sementes): " << numRepsEstatisticas << endl;
    cout << "Iterações Internas Randomizado: " << "30" << endl;
    cout << "Iterações Internas Reativo: " << reatIter << " (Bloco: " << reatBloco << ")" << endl;
    cout << "===========================================" << endl;

    // --- EXECUÇÃO DOS EXPERIMENTOS ---
    for (const string& nomeInstancia : instancias) {
        for (int d : grausTeste) {
            cout << "Executando Experimento: Instância = " << nomeInstancia 
                 << ", Grau = " << d << " ... ";
            csv << nomeInstancia << ";" << d << ";";
            string caminhoInstancia = "../instancias/dcmst/" + nomeInstancia;
            
            InstanciaDCMST instancia = LeitorInstancia::lerMatrizTriangular(caminhoInstancia, d);
                
            // 1. GULOSO (Determinístico - Roda 1 vez)
            auto t1 = chrono::high_resolution_clock::now();
            Solucao solGuloso = algoritmoGuloso(instancia.grafo, d);
            auto t2 = chrono::high_resolution_clock::now();
            double tempoGuloso = chrono::duration<double>(t2 - t1).count();
            
            double custoGuloso = solGuloso.isValida() ? solGuloso.getCusto() : -1.0;
            csv << fixed << setprecision(2) << custoGuloso << ";" << setprecision(4) << tempoGuloso << ";";

            // 2. RANDOMIZADO
            for (double alpha : alphas) {
                double melhorGlobal = 1e18, somaCustos = 0.0, somaTempos = 0.0;
                
                // "Os algoritmos devem ser executados 10 vezes... cada vez com uma semente diferente"
                for (int i = 0; i < numRepsEstatisticas; i++) {
                    // Atualiza semente (simulando seed diferente)
                    setRandomSeed(seed + i + static_cast<unsigned int>(alpha * 1000)); 

                    t1 = chrono::high_resolution_clock::now();
                    Solucao s = algoritmoGulosoRandomizado(instancia.grafo, d, alpha); 
                    t2 = chrono::high_resolution_clock::now();
                    
                    double duracao = chrono::duration<double>(t2 - t1).count();
                    somaTempos += duracao;
                    
                    if (s.isValida()) {
                        double c = s.getCusto();
                        if (c < melhorGlobal) melhorGlobal = c;
                        somaCustos += c;
                    }
                }
                
                double mediaCusto = (numRepsEstatisticas > 0) ? somaCustos / numRepsEstatisticas : -1;
                double mediaTempo = (numRepsEstatisticas > 0) ? somaTempos / numRepsEstatisticas : 0;
                if (melhorGlobal > 1e17) { melhorGlobal = -1; mediaCusto = -1; }
                
                csv << fixed << setprecision(2) << melhorGlobal << ";" << mediaCusto << ";" << setprecision(4) << mediaTempo << ";";
            }

            // 3. REATIVO
            double melhorReat = 1e18, somaReat = 0.0, somaTemposReat = 0.0;
            for (int i = 0; i < numRepsEstatisticas; i++) {
                 setRandomSeed(seed); // Semente diferente

                 t1 = chrono::high_resolution_clock::now();
                 Solucao s = algoritmoGulosoRandomizadoReativo(instancia.grafo, d, alphasReativo, reatBloco, reatIter);
                 t2 = chrono::high_resolution_clock::now();
                 
                 double duracao = chrono::duration<double>(t2 - t1).count();
                 somaTemposReat += duracao;

                 if (s.isValida()) {
                     double c = s.getCusto();
                     if (c < melhorReat) melhorReat = c;
                     somaReat += c;
                 }
            }
            double mediaReat = (numRepsEstatisticas > 0) ? somaReat / numRepsEstatisticas : -1;
            double mediaTempoReat = (numRepsEstatisticas > 0) ? somaTemposReat / numRepsEstatisticas : 0;
            if (melhorReat > 1e17) { melhorReat = -1; mediaReat = -1; }

            csv << fixed << setprecision(2) << melhorReat << ";" << mediaReat << ";" << setprecision(4) << mediaTempoReat << endl;
            
            cout << "OK" << endl;
        }
    
    }
    cout << "\nArquivo gerado: " << arquivoSaida << endl;
    csv.close();
}