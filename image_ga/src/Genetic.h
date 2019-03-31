#pragma once

#include <vector>

#include "Chromosome.h"

// TODO Pass here fitness function by reference
class Genetic {
  public:
    void evolve(int num_of_chromosomes, int crossover_rate, int mutation_rate);
  private:
    Chromosome cross(Chromosome, Chromosome);
    void selection(vector<Chromosome>);
    void crossover(vector<Chromosome>);
    void mutation(vector<Chromosome>);
}
