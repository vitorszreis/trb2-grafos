#include <vector>
#include "../include/UnionFind.h"

UnionFind::UnionFind(int n) : numConjuntos(n) {
    pai.resize(n);
    rank.resize(n, 0);
    
    for (int i = 0; i < n; i++) {
        pai[i] = i;
    }
}

int UnionFind::find(int x) {
    if (pai[x] != x) {
        pai[x] = find(pai[x]);  // Compressão de caminho
    }
    return pai[x];
}

bool UnionFind::unir(int x, int y) {
    int raizX = find(x);
    int raizY = find(y);
    
    if (raizX == raizY) {
        return false;  // Já estão conectados
    }
    
    // Union by rank
    if (rank[raizX] < rank[raizY]) {
        pai[raizX] = raizY;
    } else if (rank[raizX] > rank[raizY]) {
        pai[raizY] = raizX;
    } else {
        pai[raizY] = raizX;
        rank[raizX]++;
    }
    
    numConjuntos--;
    return true;
}

bool UnionFind::mesmoConjunto(int x, int y) {
    return find(x) == find(y);
}

int UnionFind::getNumConjuntos() const {
    return numConjuntos;
}

void UnionFind::reset() {
    numConjuntos = pai.size();
    for (std::size_t i = 0; i < pai.size(); i++) {
        pai[i] = i;
        rank[i] = 0;
    }
}