#ifndef _genetic_h_
#define _genetic_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>
#include <limits.h>
 
#include "../rules/rules.h"


#define MAX_ITERATIONS  5000
#define POPULATION_SIZE 10
#define MUTATION_RATE   0.1
#define NB_PARTICIPATION_TOURNOI 50 // doit etre > 1


/*
Grandes étapes :

1. Créer la population initiale.
2. Sélection des individus de la population qui deviendront des parents.
3. Ces parents ont des enfants qui constitueront la génération suivante.
4. Faire vivre et évoluer ces enfants.
5. Repartir au point 2 avec cette nouvelle génération.
*/



// constructeur d'individu
rules_t * genetic_create_individu();

//destructeur d'individu
void genetic_destroy_individu(rules_t * individu);

// creation de la population initiale (étape 1)
rules_t ** genetic_initialize_population();

// evaluation
int genetic_evaluate_population(int score[POPULATION_SIZE],rules_t ** population);
int genetic_evaluate_individu(rules_t * individu);

// selection + generation
int genetic_tournament_parent(int * score);
void genetic_croisement_generate_child(rules_t ** children, rules_t ** population, int p1, int p2);
void genetic_mutate(rules_t ** individu);

// resolution
rules_t * genetic_solve_optimized();

#endif
