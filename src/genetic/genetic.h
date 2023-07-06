#ifndef _genetic_h_
#define _genetic_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>
#include <limits.h>
 
#include "../rules/rules.h"
#include "../const/const.h"
#include "../simulation/simulation.h"
#include "../resolution/resolution.h"

#define MAX_ITERATIONS  3
#define POPULATION_SIZE 20
#define MUTATION_RATE 0.05
#define MUTATION_RATE_DISTANCE_FRIEND     0.03
#define MUTATION_RATE_DISTANCE_TARGET     0.03
#define MUTATION_RATE_CARDINALITY_FRIEND  0.03
#define MUTATION_RATE_CARDINALITY_TARGET  0.03
#define MUTATION_RATE_DISTANCE  0.03

#define MUTATION_RATE_ACTION   0.01
#define MUTATION_RATE_PRIORITY 0.05

#define NB_PARTICIPATION_TOURNOI 5 // doit etre > 1

rules_t * genetic_create_individu();

void genetic_destroy_individu(rules_t * individu);

void genetic_initialize_population(char * path_brain, rules_t population[POPULATION_SIZE][NB_RULES]);
void genetic_initialize_population_brain(rules_t brain[NB_RULES], rules_t population[POPULATION_SIZE][NB_RULES]);


int genetic_evaluate_population_(int score[POPULATION_SIZE],rules_t population[POPULATION_SIZE][NB_RULES]);

int genetic_evaluate_individu(rules_t * individu);

int genetic_tournament_parent(int score[POPULATION_SIZE]);

void genetic_croisement_generate_child(rules_t children[NB_RULES], rules_t population[POPULATION_SIZE][NB_RULES], int p1, int p2);

void genetic_mutate(rules_t individu[NB_RULES]);


void genetic_solve_optimized(char * path_brain_load, char * path_best_brain);
void genetic_solve_brain(rules_t brain[NB_RULES], int * score_best_brain);

#endif
