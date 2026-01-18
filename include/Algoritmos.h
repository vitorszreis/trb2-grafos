#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "Grafo.h"
#include "Solucao.h"

Solucao algoritmoGuloso(const Grafo& grafo, int d);
Solucao algoritmoGulosoRandomizado(const Grafo& grafo, int d, double alfa);

Solucao algoritmoGulosoRandomizadoReativo(const Grafo& grafo, int d,const std::vector<double>& alphas,int iteracoesPorBloco, int maxIteracoesTotal);

#endif