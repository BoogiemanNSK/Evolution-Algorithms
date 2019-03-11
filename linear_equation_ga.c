#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define VALID_SET_SIZE 10
#define NUM_OF_CHROMOSOMES 10
#define NUM_OF_GENERATIONS 1000
#define CROSSOVER_RATE 0.65
#define MUTATION_RATE 0.2
#define CHROMOSOME_SIZE 2
#define MAX_GEN_VALUE 10.0f

#define RAND_FLOAT ((float)rand()/(float)(RAND_MAX))

typedef struct {
    float gens[CHROMOSOME_SIZE];
} chromosome;

// f(0), f(1), ... , f(VALID_SET_SIZE)
float valid_set[VALID_SET_SIZE];

float f(float x, float k, float b) {
    return (k * x + b);
}

float fitness_function(chromosome *c) {
    float sum = 0.0f;
    for (int i = 0; i < VALID_SET_SIZE; i++) {
        sum += fabs(f((float)i, c->gens[0], c->gens[1]) - valid_set[i]);
    }
    return 1.0 / (1.0 + sum);
}

chromosome* best_fitness(chromosome *a, chromosome *b) {
    if (fitness_function(a) > fitness_function(b)) {
        return a;
    } else {
        return b;
    }
}

void cross(chromosome *a, chromosome *b, chromosome *child) {
    for (int i = 0; i < CHROMOSOME_SIZE; i++) {
        child->gens[i] = a->gens[i];
    }

    int cross_factor = rand() % (CHROMOSOME_SIZE + 1);
    for (int i = 0; i < CHROMOSOME_SIZE; i++) {
        if (cross_factor > i) {
            child->gens[i] = b->gens[i];
        }
    }
}

void selection(chromosome **cs) {
    float total = 0.0;
    float fitness[NUM_OF_CHROMOSOMES];
    float probability[NUM_OF_CHROMOSOMES];
    float cumulative[NUM_OF_CHROMOSOMES];
    float random_num[NUM_OF_CHROMOSOMES];

    for (int i = 0; i < NUM_OF_CHROMOSOMES; i++) {
        fitness[i] = fitness_function(cs[i]);
        total += fitness[i];
    }
    for (int i = 0; i < NUM_OF_CHROMOSOMES; i++) {
        probability[i] = fitness[i] / total;
        cumulative[i] = (i == 0) ? probability[i] : probability[i] + cumulative[i - 1];
        random_num[i] = RAND_FLOAT * 1.0f;
    }

    chromosome **new_cs = (chromosome **)malloc(sizeof(chromosome *) * NUM_OF_CHROMOSOMES);
    for (int i = 0; i < NUM_OF_CHROMOSOMES; i++) {
        int j = 0;
        while (j < NUM_OF_CHROMOSOMES && random_num[i] > cumulative[j]) { j++; }
        new_cs[i] = cs[j];
    }

    cs = new_cs;
}

void crossover(chromosome **cs) {
    for (int i = 0; i < NUM_OF_CHROMOSOMES; i++) {
        float r = RAND_FLOAT * 1.0f;
        if (r < CROSSOVER_RATE) {
            int cross_with = rand() % NUM_OF_CHROMOSOMES;

            chromosome *child = (chromosome *)malloc(sizeof(chromosome));
            
            cross(cs[i], cs[cross_with], child);
            cs[i] = best_fitness(cs[i], child);
        }
    }
}

void mutation(chromosome **cs) {
    int mutations_number = MUTATION_RATE * NUM_OF_CHROMOSOMES * CHROMOSOME_SIZE;
    for (int i = 0; i < mutations_number; i++) {
        int gen_to_mutate = rand() % (NUM_OF_CHROMOSOMES * CHROMOSOME_SIZE);
        int cs_i = gen_to_mutate / CHROMOSOME_SIZE;
        int gen_i = gen_to_mutate % CHROMOSOME_SIZE;


        chromosome *new_c = (chromosome *)malloc(sizeof(chromosome));
        
        for (int j = 0; j < CHROMOSOME_SIZE; j++) {
            new_c->gens[j] = cs[cs_i]->gens[j];
        }
        new_c->gens[gen_i] = RAND_FLOAT * MAX_GEN_VALUE;
        cs[cs_i] = best_fitness(new_c, cs[cs_i]);
    }
}

void generate_valid_set() {
    float real_k, real_b;
    
    printf("Valid k = ");
    scanf("%f", &real_k);

    printf("Valid b = ");
    scanf("%f", &real_b);

    for (int x = 0; x < VALID_SET_SIZE; x++) {
        valid_set[x] = f((float)x, real_k, real_b);
    }
}

void genetic_algorithm() {
    chromosome **cs = (chromosome **)malloc(sizeof(chromosome *) * NUM_OF_CHROMOSOMES);

    for (int i = 0; i < NUM_OF_CHROMOSOMES; i++) {
        cs[i] = (chromosome *)malloc(sizeof(chromosome));
        cs[i]->gens[0] = RAND_FLOAT * MAX_GEN_VALUE;
        cs[i]->gens[1] = RAND_FLOAT * MAX_GEN_VALUE;
    }

    for (int i = 0; i < NUM_OF_GENERATIONS; i++) {
        selection(cs);
        crossover(cs);
        mutation(cs);
    }

    int best_i = 0;
    float best_fit = 1000.0f, fit;
    for (int i = 0; i < NUM_OF_CHROMOSOMES; i++) {
        fit = fitness_function(cs[i]);
        if (fit < best_fit) {
            best_fit = fit;
            best_i = i;
        }
    }
    printf("\nResulting best chromosome after %d generations:\n", NUM_OF_GENERATIONS);
    printf("    (k = %f; b = %f)\n", cs[best_i]->gens[0], cs[best_i]->gens[1]);

    free(cs);
}

int main() {
    srand((unsigned int)time(NULL));
    printf("------| Genetic Algorithm for y=kx+b |------\n");
    
    generate_valid_set();
    genetic_algorithm();

    return 0;
}