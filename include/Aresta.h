#ifndef ARESTA_H
#define ARESTA_H

class Aresta {
private:
    int u;           // Vértice origem
    int v;           // Vértice destino
    double peso;     // Peso (custo/distância) da aresta

public:
    // Construtor
    Aresta(int u, int v, double peso);
    
    // Getters
    int getU() const;
    int getV() const;
    double getPeso() const;
    
    // Operador de comparação para ordenação (menor peso primeiro)
    bool operator<(const Aresta& outra) const;
    
    // Operador de igualdade
    bool operator==(const Aresta& outra) const;
};

#endif