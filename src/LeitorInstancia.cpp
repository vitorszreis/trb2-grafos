#include "../include/LeitorInstancia.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <cctype>
#include <algorithm>

std::string LeitorInstancia::extrairNomeArquivo(const std::string& caminho) {
    // Encontrar última barra (Windows ou Linux)
    size_t ultimaBarra = caminho.find_last_of("/\\");
    std::string nome = (ultimaBarra != std::string::npos) 
        ? caminho.substr(ultimaBarra + 1) 
        : caminho;
    
    // Remover extensão se houver
    size_t ponto = nome.find_last_of('.');
    if (ponto != std::string::npos) {
        nome = nome.substr(0, ponto);
    }
    
    return nome;
}

// Extrai o número de vértices a partir do nome da instância
// Convenção OR-Library para DC-MST:
// - O último dígito é o número da instância (0-9)
// - Os dígitos anteriores indicam o número de vértices
// - Exceção especial: "10x" = 100 vértices (crd100, str1000, etc.)
// Exemplos:
//   crd100 -> 100 vértices (exceção especial)
//   crd300 -> 30 vértices  
//   crd700 -> 70 vértices
//   str1000 -> 100 vértices (exceção especial)
//   str1500 -> 150 vértices
//   str2008 -> 200 vértices
//   sym300 -> 30 vértices
static int extrairNumVertices(const std::string& nomeInstancia) {
    // Encontrar onde começam os dígitos
    size_t inicio = 0;
    while (inicio < nomeInstancia.size() && !std::isdigit(nomeInstancia[inicio])) {
        inicio++;
    }
    
    if (inicio >= nomeInstancia.size()) {
        throw std::runtime_error("Não foi possível extrair número de vértices de: " + nomeInstancia);
    }
    
    // Extrair todos os dígitos
    std::string digitos;
    for (size_t i = inicio; i < nomeInstancia.size() && std::isdigit(nomeInstancia[i]); i++) {
        digitos += nomeInstancia[i];
    }
    
    if (digitos.empty()) {
        throw std::runtime_error("Não foi possível extrair número de vértices de: " + nomeInstancia);
    }
    
    // Exceção especial para instâncias de 100 vértices:
    // Formato "10x" significa 100 vértices
    // crd100, crd101, ..., crd109 -> 100 vértices
    // str1000, str1001, ..., str1009 -> 100 vértices  
    // shrd1000 -> 100 vértices
    if (digitos.length() == 3 && digitos[0] == '1' && digitos[1] == '0') {
        return 100;
    }
    if (digitos.length() == 4 && digitos[0] == '1' && digitos[1] == '0' && digitos[2] == '0') {
        return 100;
    }
    
    // Caso geral: remover último dígito (número da instância)
    // Os dígitos restantes são o número de vértices
    if (digitos.length() >= 2) {
        std::string verticesStr = digitos.substr(0, digitos.length() - 1);
        return std::stoi(verticesStr);
    }
    
    // Fallback: retornar o número completo
    return std::stoi(digitos);
}

// Verifica se é uma instância do tipo CRD (coordenadas euclidianas)
static bool ehInstanciaCRD(const std::string& nomeInstancia) {
    std::string nomeLower = nomeInstancia;
    std::transform(nomeLower.begin(), nomeLower.end(), nomeLower.begin(), ::tolower);
    return nomeLower.find("crd") == 0;
}

InstanciaDCMST LeitorInstancia::lerMatrizTriangular(
    const std::string& caminhoArquivo, 
    int grauMaximo
) {
    std::ifstream arquivo(caminhoArquivo);
    
    if (!arquivo.is_open()) {
        throw std::runtime_error("Erro ao abrir arquivo: " + caminhoArquivo);
    }
    
    // Extrair nome da instância
    std::string nomeInstancia = extrairNomeArquivo(caminhoArquivo);
    
    // Extrair número de vértices do nome (não contar linhas!)
    int numVertices = extrairNumVertices(nomeInstancia);
    
    // Verificar se é instância CRD (coordenadas) ou matriz triangular
    bool isCRD = ehInstanciaCRD(nomeInstancia);
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "  LEITURA DE INSTÂNCIA DC-MST" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Arquivo: " << caminhoArquivo << std::endl;
    std::cout << "Instância: " << nomeInstancia << std::endl;
    std::cout << "Vértices: " << numVertices << std::endl;
    std::cout << "Tipo: " << (isCRD ? "Coordenadas Euclidianas (CRD)" : "Matriz Triangular") << std::endl;
    std::cout << "Grau máximo (d): " << grauMaximo << std::endl;
    
    // Criar instância
    InstanciaDCMST inst(numVertices, grauMaximo, nomeInstancia);
    
    if (isCRD) {
        // ===== FORMATO CRD: Coordenadas (x, y) =====
        // Ler todas as coordenadas
        std::vector<double> coordX(numVertices);
        std::vector<double> coordY(numVertices);
        
        for (int i = 0; i < numVertices; i++) {
            if (!(arquivo >> coordX[i] >> coordY[i])) {
                throw std::runtime_error("Erro ao ler coordenadas do vértice " + std::to_string(i));
            }
        }
        
        // Calcular distâncias euclidianas e criar arestas
        for (int i = 0; i < numVertices; i++) {
            for (int j = i + 1; j < numVertices; j++) {
                double dx = coordX[i] - coordX[j];
                double dy = coordY[i] - coordY[j];
                double dist = std::sqrt(dx * dx + dy * dy);
                // Truncar para inteiro conforme especificação
                int peso = static_cast<int>(dist);
                inst.grafo.adicionarAresta(i, j, peso);
            }
        }
    } else {
        // ===== FORMATO MATRIZ TRIANGULAR INFERIOR =====
        // Formato: FOR i=1..n: FOR j=1..i-1: distance(i,j)
        // Ou seja: vértice i tem distâncias para todos os j < i
        
        for (int i = 1; i < numVertices; i++) {
            for (int j = 0; j < i; j++) {
                double peso;
                if (!(arquivo >> peso)) {
                    throw std::runtime_error("Erro ao ler peso da aresta (" + 
                        std::to_string(i) + "," + std::to_string(j) + ")");
                }
                inst.grafo.adicionarAresta(j, i, peso);
            }
        }
    }
    
    arquivo.close();
    
    // Validação
    int arestasEsperadas = (numVertices * (numVertices - 1)) / 2;
    
    std::cout << "Arestas criadas: " << inst.grafo.getNumArestas() << std::endl;
    std::cout << "Arestas esperadas: " << arestasEsperadas << " (grafo completo)" << std::endl;
    
    if (inst.grafo.getNumArestas() != arestasEsperadas) {
        std::cerr << "[AVISO] Número de arestas não corresponde ao esperado!" << std::endl;
    } else {
        std::cout << "[OK] Grafo completo carregado com sucesso!" << std::endl;
    }
        
    return inst;
}

int LeitorInstancia::obterSolucaoOtima(
    const std::string& nomeInstancia, 
    int grauMaximo
) {
    // Caminho do arquivo de soluções ótimas
    const std::string caminhoArquivo = "../instancias/dcmst/bestSolutions.txt";
    
    std::ifstream arquivo(caminhoArquivo);
    
    if (!arquivo.is_open()) {
        std::cerr << "[AVISO] Não foi possível abrir " << caminhoArquivo << std::endl;
        std::cerr << "        Não será possível calcular desvios percentuais." << std::endl;
        return -1;
    }
    
    std::string linha;
    
    // Pular cabeçalho
    std::getline(arquivo, linha);
    
    // Ler cada linha do arquivo
    while (std::getline(arquivo, linha)) {
        // Ignorar linhas vazias
        if (linha.empty() || linha.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }
        
        std::istringstream iss(linha);
        std::string instancia;
        int grau, valor;
        
        // Formato: "crd100  2    7063 H  LB=6991.56"
        // Ler: instancia, grau, valor (ignorar resto)
        if (iss >> instancia >> grau >> valor) {
            // Comparar
            if (instancia == nomeInstancia && grau == grauMaximo) {
                arquivo.close();
                return valor;
            }
        }
    }
    
    arquivo.close();
    
    // Não encontrou
    std::cerr << "[AVISO] Solução ótima não encontrada para " 
              << nomeInstancia << " com d=" << grauMaximo << std::endl;
    return -1;
}