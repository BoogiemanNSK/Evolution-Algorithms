#pragma once
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Chromosome.h"
#define RAND_FLOAT ((float)rand()/(float)(RAND_MAX))
#define RAND_INT (rand() * rand())
#define IMAGE_SIDE 512
#define RGB_MAX 255
#define SUBGENERATION_COUNT 10
#define SUBGENERATION_SIZE 40
#define CROSSOVER_RATE 0.40f								 // 40%
#define MUTATION_RATE 0.50f									 // 50%
#define MUTATION_AMOUNT 40
#define NUM_OF_GENERATIONS 80000
#define CHROMOSOME_SIDE 4
using namespace cv;
using namespace std;

// TODO Pass here fitness function by reference
class Genetic {
  public:
    Genetic(Mat InitialImage);
	void Evolve();

	vector<Vec3b> ResultingState;

  private:
	Chromosome BestFitness(Chromosome, Chromosome);
	Chromosome Cross(Chromosome, Chromosome);
	float FitnessFunction(Chromosome);
    void Selection();
    void Crossover();
    void Mutation();
	void PrintState(int, int);

	Mat Original;
	vector<Chromosome> Cs;
};