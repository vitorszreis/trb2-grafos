#include "../include/Random.h"
using namespace std;
#include <iostream>
#include <random>

mt19937 rng; // Gerador global

void setRandomSeed(unsigned int seed) {
   rng.seed(seed);
}

int randomInt(int min, int max) {
   uniform_int_distribution<int> dist(min, max);
   return dist(rng);
}
double randomDouble(double min, double max) {
   uniform_real_distribution<double> dist(min, max);
   return dist(rng);
}