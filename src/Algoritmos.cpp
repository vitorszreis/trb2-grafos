#include "../include/Algoritmos.h"
#include "../include/UnionFind.h"
#include "../include/Random.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <ctime>

#define REP_MAX 30

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

        // BUSCA LOCAL: melhora a solução até ótimo local
        solucao = buscaLocal(grafo, solucao);

        // Verifica se a solução é válida
        if (!solucao.verificarValidade()) {
            cerr << "Aviso: Randomizado não conseguiu conectar todo o grafo com d="
                << d << " e alfa=" << alfa << endl;
        }

        double custo = solucao.getCusto();
        //cout << "[Randomizado] Execução " << (rep+1) << "/" << REP_MAX << ": custo = " << custo << endl;
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

    vector<double> probabilidades(m, 1.0/m);
    vector<vector<double>> historicoQualidade(m);
    vector<int> contadorUsos(m, 0);
    vector<double> mediasQualidade(m, 0.0);
    
    Solucao melhorSolucaoGlobal(n, d);
    double melhorPesoGlobal = 1e18;

    // cout << "Probabilidades iniciais: ";
    // for(double p: probabilidades) cout << fixed << setprecision(3) << p << " ";
    // cout << endl;


    //Loop principal
    for(int iteracao = 0; iteracao < maxIteracoesTotal; iteracao++) {

    //atualização das probabilidades por bloco
        if(iteracao > 0 && iteracao % iteracoesPorBloco == 0) {
            
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
                for(int i = 0; i < m; i++) {
                    probabilidades[i] = 1.0 / m;
                }
            }
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
        
        // BUSCA LOCAL: melhora a solução até ótimo local
        solucaoAtual = buscaLocal(grafo, solucaoAtual);
        
        double pesoAtual = solucaoAtual.getCusto();


        //atualizar estatisticas
        contadorUsos[idxAlphaEscolhido]++;
        historicoQualidade[idxAlphaEscolhido].push_back(pesoAtual);

        // Calcular média para este alpha
        double somaLocal = 0.0;
        for(double peso: historicoQualidade[idxAlphaEscolhido]) {
            somaLocal += peso;
        }
        mediasQualidade[idxAlphaEscolhido] = somaLocal / historicoQualidade[idxAlphaEscolhido].size();


        //Atualiza melhor solução Global
        if(pesoAtual < melhorPesoGlobal) {
            melhorSolucaoGlobal = solucaoAtual;  
            melhorPesoGlobal = pesoAtual;        
        }
    }


    // Exibe desempenho dos alphas    
    //cout << "DESEMPENHO DOS ALPHAS:" << endl;
    for (int i = 0; i < m; i++) {
        if (contadorUsos[i] > 0) {
            double soma = 0.0;
            double melhorLocal = 1e18;
            
            for (double peso : historicoQualidade[i]) {
                soma += peso;
                if (peso < melhorLocal) melhorLocal = peso;
            }
            
            double media = soma / historicoQualidade[i].size();
            
            /*cout << "Alpha " << alphas[i] 
                << ": usado " << contadorUsos[i] << " vezes"
                << ", média: " << fixed << setprecision(2) << media
                << ", melhor: " << melhorLocal
                << ", probabilidade final: " << fixed << setprecision(3) << probabilidades[i] << endl; */
        } else {
            /*cout << "Alpha " << alphas[i] 
                << ": não foi usado" 
                << ", probabilidade final: " << fixed << setprecision(3) << probabilidades[i] << endl;*/
        }
    }
    
    // Verifica validade
    melhorSolucaoGlobal.verificarValidade();

    return melhorSolucaoGlobal;
}


Solucao buscaLocal(const Grafo& grafo, Solucao solucao) {
    int n = solucao.getNumVertices();
    int d = solucao.getGrauMaximo();
    
    if (!solucao.verificarValidade()) {
        return solucao; // Não tenta melhorar solução inválida
    }
    
    bool melhorou = true;
    
    while (melhorou) {
        melhorou = false;
        
        const vector<Aresta>& arestasAtuais = solucao.getArestas();
        int numArestas = arestasAtuais.size();
        
        // Para cada aresta da solução atual
        for (int i = 0; i < numArestas && !melhorou; i++) {
            const Aresta& arestaRemover = arestasAtuais[i];
            int u = arestaRemover.getU();
            int v = arestaRemover.getV();
            double pesoRemover = arestaRemover.getPeso();
            
            // Simular remoção: determinar componentes
            // Criar Union-Find sem a aresta (u,v)
            UnionFind uf(n);
            for (int j = 0; j < numArestas; j++) {
                if (j != i) { // Pula a aresta que estamos removendo
                    uf.unir(arestasAtuais[j].getU(), arestasAtuais[j].getV());
                }
            }
            
            // Agora u e v estão em componentes diferentes
            // Procurar aresta (a,b) que reconecta com menor custo
            const vector<Aresta>& todasArestas = grafo.getTodasArestasOrdenadas();
            
            for (const auto& arestaAdd : todasArestas) {
                int a = arestaAdd.getU();
                int b = arestaAdd.getV();
                double pesoAdd = arestaAdd.getPeso();
                
                // Só interessa se custo menor
                if (pesoAdd >= pesoRemover) {
                    continue; // Arestas estão ordenadas, pode parar se não for menor
                }
                
                // Verificar se reconecta os componentes
                if (uf.find(a) != uf.find(b)) {
                    // Verificar se já está na árvore
                    bool jaExiste = false;
                    for (const auto& existente : arestasAtuais) {
                        if ((existente.getU() == a && existente.getV() == b) ||
                            (existente.getU() == b && existente.getV() == a)) {
                            jaExiste = true;
                            break;
                        }
                    }
                    if (jaExiste) continue;
                    
                    // Verificar restrições de grau
                    int grauA = solucao.getGrau(a);
                    int grauB = solucao.getGrau(b);
                    
                    // Se a ou b era endpoint da aresta removida, seu grau será -1
                    if (a == u || a == v) grauA--;
                    if (b == u || b == v) grauB--;
                    
                    // Após adicionar (a,b), graus serão grauA+1 e grauB+1
                    if (grauA + 1 <= d && grauB + 1 <= d) {
                        // Fazer a troca!
                        solucao.removerAresta(u, v);
                        Aresta novaAresta(a, b, pesoAdd);
                        solucao.adicionarAresta(novaAresta);
                        melhorou = true;
                        break;
                    }
                }
            }
        }
    }
    
    solucao.verificarValidade();
    return solucao;
}
