#include "Genetic.h"

Genetic::Genetic(Mat initial_image) {
	Original = initial_image;
	ResultingState = vector<Vec3b>(IMAGE_SIDE * IMAGE_SIDE, Vec3b(0, 0, 0));
	srand((unsigned int)time(NULL));
}

float Genetic::FitnessFunction(Chromosome c) {
	float resultingFitness = 0.0f;

	for (int i = c.Y; i < c.Y + c.Size; i++) {
		for (int j = c.X; j < c.X + c.Size; j++) {
			Vec3b color = Original.at<Vec3b>(Point(i, j));
			resultingFitness += abs(c.R - color[0]);
			resultingFitness += abs(c.G - color[1]);
			resultingFitness += abs(c.B - color[2]);
		}
	}

	return resultingFitness;
}

void Genetic::PrintState(int i, int max) {
	system("cls");
	cout << "Evolving..." << endl << "\t[" << i + 1 << " / " << max << "]" << endl;
}

void Genetic::Evolve() {
	for (int i = 0; i < NUM_OF_GENERATIONS; i++) {
		Cs = vector<Chromosome>(SUBGENERATION_SIZE);

		for (int k = 0; k < SUBGENERATION_SIZE; k++) {
			Cs[k].R = rand() % (RGB_MAX + 1);
			Cs[k].G = rand() % (RGB_MAX + 1);
			Cs[k].B = rand() % (RGB_MAX + 1);
			Cs[k].X = rand() % (IMAGE_SIDE - CHROMOSOME_SIDE);
			Cs[k].Y = rand() % (IMAGE_SIDE - CHROMOSOME_SIDE);
			Cs[k].Size = CHROMOSOME_SIDE;
		}

		PrintState(i, NUM_OF_GENERATIONS);
		for (int k = 0; k < SUBGENERATION_COUNT; k++) {
			Selection();
			Crossover();
			Mutation();
		}

		Chromosome best = Cs[0];
		float new_fitness, best_fitness = FitnessFunction(Cs[0]);
		for (int j = 1; j < SUBGENERATION_SIZE; j++) {
			new_fitness = FitnessFunction(Cs[j]);
			if (new_fitness < best_fitness) {
				best_fitness = new_fitness;
				best = Cs[j];
			}
		}

		for (int j = best.Y; j < best.Y + best.Size; j++) {
			for (int k = best.X; k < best.X + best.Size; k++) {
				ResultingState[j * IMAGE_SIDE + k][0] = 255 - best.R;
				ResultingState[j * IMAGE_SIDE + k][1] = 255 - best.G;
				ResultingState[j * IMAGE_SIDE + k][2] = 255 - best.B;
			}
		}
	}
}

Chromosome Genetic::BestFitness(Chromosome a, Chromosome b) {
	if (FitnessFunction(a) > FitnessFunction(b)) {
		return a;
	}
	else {
		return b;
	}
}


Chromosome Genetic::Cross(Chromosome a, Chromosome b) {
	return Chromosome(
		(a.R + b.R) / 2,
		(a.G + b.G) / 2,
		(a.B + b.B) / 2,
		(a.X + b.X) / 2,
		(a.Y + b.Y) / 2,
		a.Size
	);
}

void Genetic::Selection() {
	float total = 0.0f;
	vector<float> fitness(SUBGENERATION_SIZE);
	vector<float> probability(SUBGENERATION_SIZE);
	vector<float> cumulative(SUBGENERATION_SIZE);
	vector<float> random_num(SUBGENERATION_SIZE);

	for (int i = 0; i < SUBGENERATION_SIZE; i++) {
		fitness[i] = FitnessFunction(Cs[i]);
		total += fitness[i];
	}

	for (int i = 0; i < SUBGENERATION_SIZE; i++) {
		probability[i] = fitness[i] / total;
		cumulative[i] = (i == 0) ? probability[i] : probability[i] + cumulative[i - 1];
		random_num[i] = RAND_FLOAT * 1.0f;
	}

	vector<Chromosome> newCs(SUBGENERATION_SIZE);
	for (int i = 0; i < SUBGENERATION_SIZE; i++) {
		int j = 0;
		while (j < (SUBGENERATION_SIZE - 1) && random_num[i] > cumulative[j]) { j++; }
		newCs[i] = Cs[j];
	}

	Cs = newCs;
}

void Genetic::Crossover() {
	for (int i = 0; i < SUBGENERATION_SIZE; i++) {
		float r = RAND_FLOAT * 1.0f;
		if (r < CROSSOVER_RATE) {
			int cross_with = rand() % SUBGENERATION_SIZE;

			Chromosome child = Cross(Cs[i], Cs[cross_with]);
			Cs[i] = BestFitness(Cs[i], child);
		}
	}
}

void Genetic::Mutation() {
	int mutations_number = (int)(MUTATION_RATE * (float)SUBGENERATION_SIZE * 3);

	for (int i = 0; i < mutations_number; i++) {
		int gen_to_mutate = rand() % (SUBGENERATION_SIZE * 3);
		int cs_i = gen_to_mutate / 3;
		int color_i = gen_to_mutate % 3;
		int t = rand() % 2;

		Chromosome copy = Chromosome(
			Cs[cs_i].R,
			Cs[cs_i].G,
			Cs[cs_i].B,
			Cs[cs_i].X,
			Cs[cs_i].Y,
			Cs[cs_i].Size
		);

		switch (color_i)
		{
		case 0:
			Cs[cs_i].R = t ? Cs[cs_i].R + MUTATION_AMOUNT : Cs[cs_i].R - MUTATION_AMOUNT;
			Cs[cs_i].R = Cs[cs_i].R < 0 ? 0 : Cs[cs_i].R;
			Cs[cs_i].R = Cs[cs_i].R > 255 ? 255 : Cs[cs_i].R;
			break;
		case 1:
			Cs[cs_i].G = t ? Cs[cs_i].G + MUTATION_AMOUNT : Cs[cs_i].G - MUTATION_AMOUNT;
			Cs[cs_i].G = Cs[cs_i].G < 0 ? 0 : Cs[cs_i].G;
			Cs[cs_i].G = Cs[cs_i].G > 255 ? 255 : Cs[cs_i].G;
			break;
		case 2:
			Cs[cs_i].B = t ? Cs[cs_i].B + MUTATION_AMOUNT : Cs[cs_i].B - MUTATION_AMOUNT;
			Cs[cs_i].B = Cs[cs_i].B < 0 ? 0 : Cs[cs_i].B;
			Cs[cs_i].B = Cs[cs_i].B > 255 ? 255 : Cs[cs_i].B;
			break;
		}

		Cs[cs_i] = BestFitness(Cs[cs_i], copy);
	}
}