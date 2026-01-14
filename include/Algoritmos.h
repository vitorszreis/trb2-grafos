#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "Grafo.h"
#include "Solucao.h"

Solucao algoritmoGuloso(const Grafo& grafo, int d);
Solucao algoritmoGulosoRandomizado(const Grafo& grafo, int d, double alfa);

#endif