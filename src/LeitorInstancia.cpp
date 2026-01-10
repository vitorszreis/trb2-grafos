#include "../include/LeitorInstancia.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

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
    
    // Contar número de vértices (número de linhas não vazias)
    std::string linha;
    int numVertices = 0;
    while (std::getline(arquivo, linha)) {
        // Verificar se linha não está vazia
        if (!linha.empty() && linha.find_first_not_of(" \t\r\n") != std::string::npos) {
            numVertices++;
        }
    }
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "  LEITURA DE INSTÂNCIA DC-MST" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Arquivo: " << caminhoArquivo << std::endl;
    std::cout << "Instância: " << nomeInstancia << std::endl;
    std::cout << "Vértices: " << numVertices << std::endl;
    std::cout << "Grau máximo (d): " << grauMaximo << std::endl;
    
    // Criar instância
    InstanciaDCMST inst(numVertices, grauMaximo, nomeInstancia);
    
    // Voltar ao início do arquivo
    arquivo.clear();
    arquivo.seekg(0);
    
    // Ler matriz triangular superior
    int verticeOrigem = 0;
    
    while (std::getline(arquivo, linha)) {
        // Ignorar linhas vazias
        if (linha.empty() || linha.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }
        
        std::istringstream iss(linha);
        int verticeDestino = verticeOrigem + 1;
        double peso;
        
        // Ler todos os pesos da linha
        while (iss >> peso) {
            if (verticeDestino < numVertices) {
                inst.grafo.adicionarAresta(verticeOrigem, verticeDestino, peso);
                verticeDestino++;
            }
        }
        
        verticeOrigem++;
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