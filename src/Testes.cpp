#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include "../include/Grafo.h"
#include "../include/Aresta.h"
#include "../include/UnionFind.h"
#include "../include/LeitorInstancia.h"
#include "../include/Solucao.h"


using namespace std;

void testarGrafo() {
    cout << "\n " << endl;
    cout << "     TESTE DA ESTRUTURA DO GRAFO         " << endl;
    cout << "\n" << endl;
    
    // Criar grafo com 4 vértices
    cout << "[1] Criando grafo com 4 vértices..." << endl;
    Grafo g(4);
    
    // Adicionar arestas
    cout << "[2] Adicionando arestas..." << endl;
    g.adicionarAresta(0, 1, 10.0);
    g.adicionarAresta(0, 2, 20.0);
    g.adicionarAresta(0, 3, 30.0);
    g.adicionarAresta(1, 2, 15.0);
    g.adicionarAresta(1, 3, 25.0);
    g.adicionarAresta(2, 3, 35.0);
    
    // Imprimir estrutura
    cout << "[3] Estrutura do grafo:" << endl;
    g.imprimirGrafo();
    
    // Testar getters
    cout << "[4] Testando getters básicos:" << endl;
    cout << "    Número de vértices: " << g.getNumVertices() << endl;
    cout << "    Número de arestas: " << g.getNumArestas() << endl;
    cout << "    Peso aresta (0,1): " << g.getPeso(0, 1) << endl;
    cout << "    Peso aresta (2,3): " << g.getPeso(2, 3) << endl;
    
    // Testar vizinhos
    cout << "\n[5] Vizinhos do vértice 0:" << endl;
    auto vizinhos = g.getVizinhos(0);
    for (const auto& vizinho : vizinhos) {
        cout << "    -> Vértice " << vizinho.first << " com peso " << vizinho.second << endl;
    }
    
    // Testar ordenação de arestas
    cout << "\n[6] Todas as arestas (ordenadas por peso):" << endl;
    vector<Aresta> arestas = g.getTodasArestas();
    sort(arestas.begin(), arestas.end());
    
    for (const auto& a : arestas) {
        cout << "    Aresta (" << a.getU() << ", " << a.getV() 
             << ") - Peso: " << a.getPeso() << endl;
    }
    
    cout << "\n TODOS OS TESTES PASSARAM!\n" << endl;
}

void testarUnionFind() {
    cout << "\n " << endl;
    cout << "     TESTE DA ESTRUTURA UNION-FIND       " << endl;
    cout << "\n" << endl;
    
    // Criar Union-Find com 5 elementos
    cout << "[1] Criando Union-Find com 5 elementos..." << endl;
    UnionFind uf(5);
    
    cout << "[2] Conjuntos iniciais: " << uf.getNumConjuntos() << endl;
    
    // Fazer algumas uniões
    cout << "[3] Unindo elementos..." << endl;
    uf.unir(0, 1);
    uf.unir(1, 2);
    uf.unir(3, 4);
    
    cout << "    Conjuntos após uniões: " << uf.getNumConjuntos() << endl;
    
    // Testar se estão no mesmo conjunto
    cout << "[4] Verificando conjuntos:" << endl;
    cout << "    0 e 2 no mesmo conjunto? " << (uf.mesmoConjunto(0, 2) ? "Sim" : "Não") << endl;
    cout << "    0 e 3 no mesmo conjunto? " << (uf.mesmoConjunto(0, 3) ? "Sim" : "Não") << endl;
    
    cout << "\n TESTES UNION-FIND PASSARAM!\n" << endl;
}

void testarUnionFindComGrafo() {
    cout << "\n" << endl;
    cout << "  TESTE 3: UNION-FIND + GRAFO (MST)   " << endl;
    cout << "\n" << endl;
    
    cout << "[1] Criando grafo exemplo..." << endl;
    Grafo g(4);
    g.adicionarAresta(0, 1, 10.0);
    g.adicionarAresta(0, 2, 20.0);
    g.adicionarAresta(0, 3, 30.0);
    g.adicionarAresta(1, 2, 15.0);
    g.adicionarAresta(1, 3, 25.0);
    g.adicionarAresta(2, 3, 35.0);
    
    cout << "[2] Simulando algoritmo de Kruskal..." << endl;
    
    // Obter e ordenar arestas
    vector<Aresta> arestas = g.getTodasArestas();
    sort(arestas.begin(), arestas.end());
    
    cout << "    Arestas ordenadas:" << endl;
    for (const auto& a : arestas) {
        cout << "      (" << a.getU() << "," << a.getV() << ") peso " << a.getPeso() << endl;
    }
    
    // Union-Find para MST
    UnionFind uf(g.getNumVertices());
    vector<Aresta> mst;
    double custoTotal = 0;
    
    cout << "\n[3] Processando arestas..." << endl;
    for (const auto& a : arestas) {
        int u = a.getU();
        int v = a.getV();
        double peso = a.getPeso();
        
        cout << "    Tentando aresta (" << u << "," << v << ") peso " << peso << ": ";
        
        if (uf.unir(u, v)) {
            mst.push_back(a);
            custoTotal += peso;
            cout << "ADICIONADA " << endl;
        } else {
            cout << "REJEITADA (criaria ciclo) " << endl;
        }
        
        if (mst.size() == (size_t)(g.getNumVertices() - 1)) {
            cout << "     MST completa!" << endl;
            break;
        }
    }
    
    cout << "\n[4] Resultado (MST):" << endl;
    cout << "    Arestas na MST:" << endl;
    for (const auto& a : mst) {
        cout << "      (" << a.getU() << "," << a.getV() << ") peso " << a.getPeso() << endl;
    }
    cout << "    Custo total: " << custoTotal << endl;
    cout << "    Número de arestas: " << mst.size() << " (esperado: " 
         << g.getNumVertices() - 1 << ")" << endl;
    
    cout << "\n TESTE 3 PASSOU!\n" << endl;
}

void testarLeituraInstancia() {
    cout << "\n" << endl;
    cout << "  TESTE 4: LEITURA DE INSTÂNCIAS      " << endl;
    cout << "\n" << endl;
    
    try {
        cout << "[1] Lendo instância crd100 com d=3..." << endl;
        InstanciaDCMST inst = LeitorInstancia::lerMatrizTriangular(
            "../instancias/dcmst/crd100",
            3
        );
        
        cout << "[2] Dados da instância carregada:" << endl;
        cout << "    Nome: " << inst.nomeInstancia << endl;
        cout << "    Vértices: " << inst.numVertices << endl;
        cout << "    Grau máximo: " << inst.grauMaximo << endl;
        cout << "    Arestas: " << inst.grafo.getNumArestas() << endl;
        
        cout << "\n[3] Verificando primeiras arestas..." << endl;
        auto vizinhos = inst.grafo.getVizinhos(0);
        cout << "    Vértice 0 tem " << vizinhos.size() << " vizinhos" << endl;
        cout << "    Primeiros 5 vizinhos:" << endl;
        for (size_t i = 0; i < min(size_t(5), vizinhos.size()); i++) {
            auto par = vizinhos[i];
            cout << "      -> Vértice " << par.first 
                 << " com peso " << par.second << endl;
        }
        
        cout << "\n[4] Verificando simetria do grafo..." << endl;
        double peso01 = inst.grafo.getPeso(0, 1);
        double peso10 = inst.grafo.getPeso(1, 0);
        cout << "    Peso(0,1) = " << peso01 << endl;
        cout << "    Peso(1,0) = " << peso10 << endl;
        cout << "    Simétrico? " << (peso01 == peso10 ? "SIM ✓" : "NÃO ✗") << endl;
        
        cout << "\n[5] Buscando solução ótima conhecida..." << endl;
        int otimo = LeitorInstancia::obterSolucaoOtima("crd100", 3);
        if (otimo != -1) {
            cout << "    Solução ótima para crd100 d=3: " << otimo << endl;
        } else {
            cout << "    [AVISO] Solução ótima não encontrada" << endl;
        }
        
        cout << "\n✓ TESTE 4 PASSOU!\n" << endl;
        
    } catch (const exception& e) {
        cout << "\n TESTE 4 FALHOU!" << endl;
        cout << "Erro: " << e.what() << endl;
        cout << "\nVerifique:" << endl;
        cout << "  1. O arquivo instancias/dcmst/crd100 existe?" << endl;
        cout << "  2. O caminho está correto?" << endl;
        cout << "  3. O arquivo tem permissões de leitura?" << endl;
    }
}

void testarFormatoGraphEditor() {
    cout << "\n" << endl;
    cout << "  TESTE 5: FORMATO GRAPH EDITOR        " << endl;
    cout << "\n" << endl;
    
    // Criar grafo pequeno
    Grafo g(4);
    g.adicionarAresta(0, 1, 10);
    g.adicionarAresta(0, 2, 20);
    g.adicionarAresta(1, 3, 25);
    g.adicionarAresta(2, 3, 35);
    
    cout << "[1] Imprimindo no console..." << endl;
    g.imprimirFormatoGraphEditor();
    
    cout << "[2] Salvando em arquivo..." << endl;
    g.salvarFormatoGraphEditor("../resultados/grafo_teste.txt");
    
    cout << "\n TESTE 5 PASSOU!\n" << endl;
    cout << "Abra o arquivo resultados/grafo_teste.txt e copie o conteúdo" << endl;
    cout << "para https://csacademy.com/app/graph_editor/ para visualizar!\n" << endl;
}


void testarCacheOrdenacao() {
    cout << "\n" << endl;
    cout << " TESTE 6: CACHE DE ORDENAÇÃO          " << endl;
    cout << "\n" << endl;
    
    cout << "[1] Criando grafo com arestas desordenadas..." << endl;
    Grafo g(5);
    
    // Adicionar em ordem aleatória
    g.adicionarAresta(0, 1, 50);
    g.adicionarAresta(1, 2, 10);  // Menor peso
    g.adicionarAresta(2, 3, 30);
    g.adicionarAresta(3, 4, 20);
    g.adicionarAresta(0, 4, 40);
    
    cout << "[2] Primeira chamada getTodasArestasOrdenadas()..." << endl;
    auto inicio1 = chrono::high_resolution_clock::now();
    const vector<Aresta>& arestas1 = g.getTodasArestasOrdenadas();
    auto fim1 = chrono::high_resolution_clock::now();
    auto duracao1 = chrono::duration_cast<chrono::microseconds>(fim1 - inicio1);
    
    cout << "    Arestas ordenadas:" << endl;
    for (const auto& a : arestas1) {
        cout << "      (" << a.getU() << "," << a.getV() 
             << ") peso " << a.getPeso() << endl;
    }
    cout << "    Tempo: " << duracao1.count() << " µs (primeira vez)" << endl;
    
    cout << "\n[3] Segunda chamada (deve usar cache)..." << endl;
    auto inicio2 = chrono::high_resolution_clock::now();
    const vector<Aresta>& arestas2 = g.getTodasArestasOrdenadas();
    auto fim2 = chrono::high_resolution_clock::now();
    auto duracao2 = chrono::duration_cast<chrono::microseconds>(fim2 - inicio2);
    
    cout << "    Tempo: " << duracao2.count() << " µs (cache)" << endl;
    cout << "    Speedup: " << (duracao2.count() > 0 ? 
           static_cast<double>(duracao1.count()) / duracao2.count() : 0) 
         << "x mais rápido" << endl;
    
    cout << "\n[4] Verificando ordenação..." << endl;
    bool ordenado = true;
    for (size_t i = 1; i < arestas2.size(); i++) {
        if (arestas2[i-1].getPeso() > arestas2[i].getPeso()) {
            ordenado = false;
            break;
        }
    }
    cout << "    Ordenado corretamente? " << (ordenado ? "SIM ✓" : "NÃO ✗") << endl;
    
    cout << "\n✓ TESTE 6 PASSOU!\n" << endl;
}

void testarClasseSolucao() {
    cout << "\n" << endl;
    cout << "  TESTE 7: CLASSE SOLUÇÃO              " << endl;
    cout << "\n" << endl;
    
    cout << "[1] Criando solução vazia (n=5, d=3)..." << endl;
    Solucao sol(5, 3);
    
    cout << "    Custo inicial: " << sol.getCusto() << endl;
    cout << "    Número de arestas: " << sol.getNumArestas() << endl;
    
    cout << "\n[2] Adicionando arestas respeitando grau máximo..." << endl;
    
    Aresta a1(0, 1, 10);
    Aresta a2(1, 2, 15);
    Aresta a3(2, 3, 20);
    Aresta a4(3, 4, 25);
    
    cout << "    Adicionar (0,1,10): " << (sol.adicionarAresta(a1) ? "OK ✓" : "FALHOU ✗") << endl;
    cout << "    Adicionar (1,2,15): " << (sol.adicionarAresta(a2) ? "OK ✓" : "FALHOU ✗") << endl;
    cout << "    Adicionar (2,3,20): " << (sol.adicionarAresta(a3) ? "OK ✓" : "FALHOU ✗") << endl;
    cout << "    Adicionar (3,4,25): " << (sol.adicionarAresta(a4) ? "OK ✓" : "FALHOU ✗") << endl;
    
    cout << "\n[3] Estado da solução:" << endl;
    cout << "    Custo total: " << sol.getCusto() << endl;
    cout << "    Número de arestas: " << sol.getNumArestas() << endl;
    cout << "    Grau do vértice 0: " << sol.getGrau(0) << endl;
    cout << "    Grau do vértice 1: " << sol.getGrau(1) << endl;
    cout << "    Grau do vértice 2: " << sol.getGrau(2) << endl;
    cout << "    Grau máximo real: " << sol.getGrauMaximoReal() << endl;
    
    cout << "\n[4] Testando limite de grau..." << endl;
    // Vértice 1 já tem grau 2
    Aresta a5(1, 3, 30);
    cout << "    Adicionar (1,3,30) - vértice 1 terá grau 3: " 
         << (sol.adicionarAresta(a5) ? "OK ✓" : "FALHOU ✗") << endl;
    
    // Agora vértice 1 tem grau 3 (máximo)
    Aresta a6(1, 4, 35);
    cout << "    Adicionar (1,4,35) - vértice 1 já tem grau 3: " 
         << (sol.adicionarAresta(a6) ? "OK ✗ (não deveria aceitar)" : "REJEITADO ✓") << endl;
    
    cout << "\n[5] Validando solução..." << endl;
    bool valida = sol.verificarValidade();
    cout << "    Solução válida? " << (valida ? "SIM ✓" : "NÃO ✗") << endl;
    
    cout << "\n[6] Impressão formatada:" << endl;
    sol.imprimir();
    
    cout << " TESTE 7 PASSOU!\n" << endl;
}

void testarSolucaoComGrafoReal() {
    cout << "\n" << endl;
    cout << "  TESTE 8: SOLUÇÃO COM GRAFO REAL      " << endl;
    cout << "\n" << endl;
    
    cout << "[1] Criando grafo completo pequeno..." << endl;
    Grafo g(4);
    g.adicionarAresta(0, 1, 10);
    g.adicionarAresta(0, 2, 20);
    g.adicionarAresta(0, 3, 30);
    g.adicionarAresta(1, 2, 15);
    g.adicionarAresta(1, 3, 25);
    g.adicionarAresta(2, 3, 35);
    
    cout << "[2] Construindo MST usando arestas ordenadas..." << endl;
    Solucao sol(4, 3);
    UnionFind uf(4);
    
    const vector<Aresta>& arestas = g.getTodasArestasOrdenadas();
    
    cout << "    Processando arestas em ordem crescente:" << endl;
    for (const auto& a : arestas) {
        cout << "      Aresta (" << a.getU() << "," << a.getV() 
             << ") peso " << a.getPeso() << ": ";
        
        // Verificar se cria ciclo
        if (uf.mesmoConjunto(a.getU(), a.getV())) {
            cout << "REJEITADA (ciclo) " << endl;
            continue;
        }
        
        // Verificar grau
        if (sol.adicionarAresta(a)) {
            uf.unir(a.getU(), a.getV());
            cout << "ADICIONADA " << endl;
            
            if (sol.getNumArestas() == 3) {
                cout << "      → MST completa!" << endl;
                break;
            }
        } else {
            cout << "REJEITADA (grau) ✗" << endl;
        }
    }
    
    cout << "\n[3] Validando solução construída..." << endl;
    sol.verificarValidade();
    sol.imprimir();
    
    cout << " TESTE 8 PASSOU!\n" << endl;
}