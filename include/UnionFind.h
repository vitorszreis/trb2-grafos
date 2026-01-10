#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <vector>

/**
 * Estrutura Union-Find (Disjoint Set Union)
 * 
 * Mantém conjuntos disjuntos de elementos e permite:
 * - find(x): Encontrar o representante (raiz) do conjunto de x
 * - unir(x, y): Unir os conjuntos que contêm x e y
 * 
 * Otimizações implementadas:
 * - Compressão de caminho (Path Compression) no find
 * - União por rank (Union by Rank) no unir
 * 
 * Complexidade: O(α(n)) amortizado ≈ O(1) na prática
 * 
 */

class UnionFind {
private:
    std::vector<int> pai;
    std::vector<int> rank;
    int numConjuntos;

public:
    UnionFind(int n);
    
    // Encontra representante do conjunto (com compressão de caminho)
    int find(int x);
    
    // Une dois conjuntos (retorna false se já estavam unidos)
    bool unir(int x, int y);
    
    // Verifica se x e y estão no mesmo conjunto
    bool mesmoConjunto(int x, int y);
    
    int getNumConjuntos() const;
    
    void reset();
};

#endif