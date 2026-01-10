#include "../include/Aresta.h"

// Construtor
Aresta::Aresta(int u, int v, double peso) 
    : u(u), v(v), peso(peso) {}

// Getters
int Aresta::getU() const {
    return u;
}

int Aresta::getV() const {
    return v;
}

double Aresta::getPeso() const {
    return peso;
}

// Operador < para ordenação (ordem crescente de peso)
bool Aresta::operator<(const Aresta& outra) const {
    return peso < outra.peso;
}

// Operador de igualdade
bool Aresta::operator==(const Aresta& outra) const {
    return (u == outra.u && v == outra.v && peso == outra.peso) ||
           (u == outra.v && v == outra.u && peso == outra.peso);
}