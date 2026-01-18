#include "../include/Algoritmos.h"
#include "../include/UnionFind.h"
#include "../include/Random.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <ctime>

#define REP_MAX 10

using namespace std;

Solucao algoritmoGuloso(const Grafo& grafo, int d) {
    int n = grafo.getNumVertices();
    
    Solucao solucao(n, d);
    UnionFind uf(n);
    
    // arestas já ordenadas
    const vector<Aresta>& arestas = grafo.getTodasArestasOrdenadas();
    
    // itera sobre arestas)
    int arestasAdicionadas = 0;
    
    for (const auto& aresta : arestas) {
        // critério de parada: se já temos n-1 arestas, a árvore está pronta
        if (arestasAdicionadas == n - 1) break;
        
        int u = aresta.getU();
        int v = aresta.getV();
        
        
        //verificação do union-find
        if (uf.find(u) == uf.find(v)) {
            continue; // se já estão no mesmo conjunto, formaria ciclo -> descarta
        }
        
        // verificação de restrição de grau (degree constraint)
        // só adicionamos a aresta se os vértices tiverem grau menor que d
        if (solucao.getGrau(u) < d && solucao.getGrau(v) < d) {
            if(solucao.adicionarAresta(aresta)) {
                // une os conjuntos no union-find para marcar que u e v estão conectados
                uf.unir(u, v);
                arestasAdicionadas++;
            }
        }
    }
    
    // verifica se a solução é válida (conexa e com n-1 arestas)
    if (!solucao.verificarValidade()) {
        cerr << "Aviso: Guloso não conseguiu conectar todo o grafo com d=" << d << endl;
    }
    
    return solucao;
}

Solucao algoritmoGulosoRandomizado(const Grafo& grafo, int d, double alfa) {
    int n = grafo.getNumVertices();
    Solucao melhorSolucao(n, d);
    double melhorCusto = 1e18;

    for (int rep = 0; rep < REP_MAX; ++rep) {
        Solucao solucao(n, d);
        UnionFind uf(n);

        // Arestas  ordenadas por peso
        const vector<Aresta>& arestas = grafo.getTodasArestasOrdenadas();
        int arestasAdicionadas = 0;

        while (arestasAdicionadas < n - 1) {
            // Monta lista de candidatos viáveis
            vector<Aresta> candidatos;
            double custoMin = 1e9, custoMax = -1e9;

            for (const auto& a : arestas) {
                int u = a.getU(), v = a.getV();
                if (uf.find(u) != uf.find(v) &&
                    solucao.getGrau(u) < d &&
                    solucao.getGrau(v) < d) {
                    candidatos.push_back(a);
                    double peso = a.getPeso();
                    custoMin = min(custoMin, peso);
                    custoMax = max(custoMax, peso);
                }
            }

            if (candidatos.empty()) {
                cerr << "Não foi possível adicionar mais arestas sem violar o limite de grau." << endl;
                break;
            }

            // Lista de Candidatos Restritos
            vector<Aresta> rcl;
            double limite = custoMin + alfa * (custoMax - custoMin);
            for (const auto& a : candidatos) {
                if (a.getPeso() <= limite) {
                    rcl.push_back(a);
                }
            }

            if (rcl.empty()) {
                cerr << "RCL vazia! Ajuste o valor de alfa." << endl;
                break;
            }

            // Escolhe aleatoriamente uma aresta da RCL
            int idx = randomInt(0, rcl.size() - 1);
            const Aresta& escolhida = rcl[idx];

            int u = escolhida.getU();
            int v = escolhida.getV();
            if (solucao.adicionarAresta(escolhida)) {
                uf.unir(u, v);
                arestasAdicionadas++;
            }
        }

        // Verifica se a solução é válida
        if (!solucao.verificarValidade()) {
            cerr << "Aviso: Randomizado não conseguiu conectar todo o grafo com d="
                << d << " e alfa=" << alfa << endl;
        }

        double custo = solucao.getCusto();
        cout << "[Randomizado] Execução " << (rep+1) << "/" << REP_MAX << ": custo = " << custo << endl;
        if (solucao.verificarValidade() && custo < melhorCusto) {
            melhorCusto = custo;
            melhorSolucao = solucao;
        }
    }
    return melhorSolucao;
}
Solucao algoritmoGulosoRandomizadoReativo(const Grafo& grafo, int d, const std::vector<double>& alphas,int iteracoesPorBloco,int maxIteracoesTotal) {
    
    int n = grafo.getNumVertices();  
    int m = alphas.size(); 

    cout << "\n=======================================" << endl;
    cout << "ALGORITMO GULOSO RANDOMIZADO REATIVO" << endl;
    cout << "=======================================" << endl;
    cout << "Vértices: " << n << endl;
    cout << "Grau máximo: d = " << d << endl;
    cout << "Alphas: ";

    for(double a: alphas) cout << a << " ";
    cout << endl;
    cout << "Iterações por bloco: " << iteracoesPorBloco << endl;
    cout << "Iterações totais: " << maxIteracoesTotal << endl;
    cout << "=======================================\n" << endl;

  
    vector<double> probabilidades(m, 1.0/m);
    vector<vector<double>> historicoQualidade(m);
    vector<int> contadorUsos(m, 0);
    vector<double> mediasQualidade(m, 0.0);
    
    Solucao melhorSolucaoGlobal(n, d);
    double melhorPesoGlobal = 1e18;

    cout << "Estruturas inicializadas com sucesso!" << endl;
    cout << "Probabilidades iniciais: ";
    for(double p: probabilidades) cout << fixed << setprecision(3) << p << " ";
    cout << endl;

    cout << "\nIniciando loop principal" << endl;

//Loop principal
    for(int iteracao = 0; iteracao < maxIteracoesTotal; iteracao++) {
        cout << "\n--- Iteração " << (iteracao + 1) << " ---" << endl;

//atualização das probabilidades por bloco
        if(iteracao > 0 && iteracao % iteracoesPorBloco == 0) {
            cout << "\n[Bloco " << (iteracao / iteracoesPorBloco)
                << "] Atualizando probabilidades..." << endl;
            
            vector<double> medias(m, 0.0);

            for(int i = 0; i < m; i++) {
                if(contadorUsos[i] > 0) {
                    double soma = 0.0;
                    for(double peso : historicoQualidade[i]) {
                        soma += peso;
                    }
                    medias[i] = soma / historicoQualidade[i].size();
                } else {
                    medias[i] = 1e18;
                }
            }

            vector<double> qValues(m, 0.0);
            double somaQ = 0.0;

            for(int i = 0; i < m; i++) {
                if(contadorUsos[i] > 0 && medias[i] > 0 && medias[i] < 1e17) {
                    qValues[i] = melhorPesoGlobal / medias[i];
                    somaQ += qValues[i];
                }
            }
            
            if(somaQ > 0) {
                for(int i = 0; i < m; i++) {
                    probabilidades[i] = qValues[i] / somaQ;
                }
            } else {
                cout << "  Aviso: Todos q_i são zero, mantendo probabilidades uniformes" << endl;
                for(int i = 0; i < m; i++) {
                    probabilidades[i] = 1.0 / m;
                }
            }

            cout << "  Novas probabilidades: ";
            for(int i = 0; i < m; i++) {
                cout << "α " << alphas[i] << "= " << fixed << setprecision(3) << probabilidades[i] << " ";
            }
            cout << endl;

            cout << "  Médias dos alphas: ";
            for(int i = 0; i < m; i++) {
                if(contadorUsos[i] > 0) {
                    cout << "α " << alphas[i] << "= " << fixed << setprecision(2) << medias[i] << " ";
                } else {
                    cout << "α " << alphas[i] << "= N/A ";
                }
            }
            cout << endl;
        }
//Escolha do Alpha
        double alphaEscolhido;
        int idxAlphaEscolhido;

        double r = randomDouble(0.0, 1.0);
        double acumulado = 0.0;
        bool escolhido = false;

        for(int i = 0; i < m; i++) {
            acumulado += probabilidades[i];
            if(r <= acumulado) {
                alphaEscolhido = alphas[i];
                idxAlphaEscolhido = i;
                escolhido = true;
                break;
            }
        }

        if(!escolhido) {
            alphaEscolhido = alphas[m - 1];
            idxAlphaEscolhido = m - 1;
        }

        cout << "  Alpha escolhido: " << alphaEscolhido
             << " (probabilidade: " << fixed << setprecision(3)
             << probabilidades[idxAlphaEscolhido] << ")" << endl;

//Função para construir a solução gulosa randomizada (la,bda)
        auto construirSolucaoGulosa = [&](double alpha) -> Solucao {
            int n_local = grafo.getNumVertices();
            Solucao solucao(n_local, d);
            UnionFind uf(n_local);
            
            const vector<Aresta>& arestas = grafo.getTodasArestasOrdenadas();
            int arestasAdicionadas = 0;
            
            while (arestasAdicionadas < n_local - 1) {
                // Monta lista de candidatos viáveis
                vector<Aresta> candidatos;
                double custoMin = 1e9, custoMax = -1e9;
                
                for (const auto& a : arestas) {
                    int u = a.getU(), v = a.getV();
                    if (uf.find(u) != uf.find(v) &&
                        solucao.getGrau(u) < d &&
                        solucao.getGrau(v) < d) {
                        candidatos.push_back(a);
                        double peso = a.getPeso();
                        custoMin = min(custoMin, peso);
                        custoMax = max(custoMax, peso);
                    }
                }
                
                if (candidatos.empty()) {
                    break;
                }
                
                // Monta RCL (Lista de Candidatos Restritos)
                vector<Aresta> rcl;
                double limite = custoMin + alpha * (custoMax - custoMin);
                for (const auto& a : candidatos) {
                    if (a.getPeso() <= limite) {
                        rcl.push_back(a);
                    }
                }
                
                if (rcl.empty()) {
                    break;
                }
                
                // Escolhe aleatoriamente uma aresta da RCL
                int idx = randomInt(0, rcl.size() - 1);
                const Aresta& escolhida = rcl[idx];
                
                int u = escolhida.getU();
                int v = escolhida.getV();
                if (solucao.adicionarAresta(escolhida)) {
                    uf.unir(u, v);
                    arestasAdicionadas++;
                }
            }
            
            return solucao;
        };

       //construção da solução
        Solucao solucaoAtual = construirSolucaoGulosa(alphaEscolhido);
        double pesoAtual = solucaoAtual.getCusto();

        cout << "  Peso da solução: " << pesoAtual << endl;

        //atualizar estatisticas
        contadorUsos[idxAlphaEscolhido]++;
        historicoQualidade[idxAlphaEscolhido].push_back(pesoAtual);

        // Calcular média para este alpha
        double somaLocal = 0.0;
        for(double peso: historicoQualidade[idxAlphaEscolhido]) {
            somaLocal += peso;
        }
        mediasQualidade[idxAlphaEscolhido] = somaLocal / historicoQualidade[idxAlphaEscolhido].size();

        cout << "  Média atual deste alpha: " << mediasQualidade[idxAlphaEscolhido] << endl;

//Atualiza melhor solução Global
        if(pesoAtual < melhorPesoGlobal) {
            melhorSolucaoGlobal = solucaoAtual;  
            melhorPesoGlobal = pesoAtual;        
            cout << "  *** NOVA MELHOR SOLUÇÃO ENCONTRADA! ***" << endl;
            cout << "      Peso: " << pesoAtual << endl;
            cout << "      Alpha usado: " << alphaEscolhido << endl;
        }

    //log de Progresso
        if((iteracao + 1) % 10 == 0) {
            cout << "\n[Progresso] Iteração " << setw(3) << (iteracao + 1) 
                 << "/" << maxIteracoesTotal << endl;
            cout << "  Melhor peso até agora: " << melhorPesoGlobal << endl;
            cout << "  Distribuição de uso dos Alphas: ";
            for(int i = 0; i < m; i++) {
                cout << "α " << alphas[i] << ": " << contadorUsos[i] << " ";
            }
            cout << endl;
        }
    }

   //Relatorio Final
    cout << "\n========================================" << endl;
    cout << "         RELATÓRIO FINAL               " << endl;
    cout << "========================================" << endl;
    
    cout << "\nDESEMPENHO DOS ALPHAS:" << endl;
    for (int i = 0; i < m; i++) {
        if (contadorUsos[i] > 0) {
            double soma = 0.0;
            double melhorLocal = 1e18;
            
            for (double peso : historicoQualidade[i]) {
                soma += peso;
                if (peso < melhorLocal) melhorLocal = peso;
            }
            
            double media = soma / historicoQualidade[i].size();
            
            cout << "Alpha " << alphas[i] 
                << ": usado " << contadorUsos[i] << " vezes"
                << ", média: " << fixed << setprecision(2) << media
                << ", melhor: " << melhorLocal
                << ", probabilidade final: " << fixed << setprecision(3) << probabilidades[i] << endl;
        } else {
            cout << "Alpha " << alphas[i] 
                << ": não foi usado" 
                << ", probabilidade final: " << fixed << setprecision(3) << probabilidades[i] << endl;
        }
    }
    
    // Verifica validade
    melhorSolucaoGlobal.verificarValidade();
    
    cout << "\nMELHOR SOLUÇÃO ENCONTRADA:" << endl;
    cout << "  Peso total: " << fixed << setprecision(2) << melhorPesoGlobal << endl;
    cout << "  Solução válida: " << (melhorSolucaoGlobal.isValida() ? "SIM ✓" : "NÃO ✗") << endl;
    cout << "========================================\n" << endl;

    return melhorSolucaoGlobal;
}
