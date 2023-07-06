#include "genetic.h"


/**
 * @brief Initializes the population by saving the rules to a file.
 *
 * This function initializes the population by saving the rules for each individual in the
 * population to a file. It uses the `rules_save_file` function to save the rules of each
 * individual in the provided `population` array to the given file.
 *
 * @param file Pointer to the file where the rules will be saved.
 * @param population Array representing the population of individuals.
 *
 * @note The file must be opened in write mode before calling this function.
 */
void genetic_initialize_population(char * path_brain, rules_t population[POPULATION_SIZE][NB_RULES])
{
    rules_read_path_file(path_brain, population[0]);
    for (int i=1; i < POPULATION_SIZE; ++i)
    {
	rules_copy_brain(population[0],population[i]);
	genetic_mutate(population[i]);
        /* resolution_random_change(population[i]); */
    }
}


void genetic_initialize_population_brain(rules_t brain[NB_RULES], rules_t population[POPULATION_SIZE][NB_RULES])
{
    rules_copy_brain(brain, population[0]);
    for (int i=1; i < POPULATION_SIZE; ++i)
    {
	rules_copy_brain(population[0], population[i]);
	genetic_mutate(population[i]);
    }
}

/**
 * @brief Evaluates the fitness of each individual in the population and returns the index of the best individual.
 *
 * This function evaluates the fitness of each individual in the population by calling the `genetic_evaluate_individu` function
 * for each individual. The fitness score for each individual is stored in the `score` array. The function then determines the
 * index of the individual with the highest fitness score and returns it as the best individual.
 *
 * @param score An array to store the fitness scores of each individual in the population. The array should have a size of `POPULATION_SIZE`.
 * @param population A pointer to an array of pointers to `rules_t` structures representing the population of individuals.
 *
 * @return The index of the best individual in the population.
 */
int genetic_evaluate_population(int score[POPULATION_SIZE],rules_t population[POPULATION_SIZE][NB_RULES])
{
    int best_individu = INT_MAX;
    int index_best_individu = 0;
    for (int i=0; i < POPULATION_SIZE; ++i)
    {
	zlog(stdout, DEBUG, "evaluation score de l'individu %d", i);
	score[i] = genetic_evaluate_individu(population[i]);
	zlog(stdout, DEBUG, "l'individu %d a un score de %d", i, score[i]);
	if (score[i] < best_individu)
        {
            best_individu = score[i];
            index_best_individu = i;    
        }
    }
    return index_best_individu;
}

/**
 * @brief Evaluates an individual for genetic algorithms.
 *
 * This function evaluates the fitness of an individual in the context of genetic algorithms.
 * The fitness evaluation typically involves analyzing the characteristics or performance of
 * the individual based on specific criteria or objectives defined by the genetic algorithm.
 *
 * @param individu A pointer to the individual to be evaluated.
 * @return The fitness value of the individual.
 */
int genetic_evaluate_individu(rules_t * individu)
{
    int score = INT_MAX;
    //simulation_loop(individu, &score);
    simulation_loop_average(individu, &score);
    /* rules_save_file(stdout,individu); */
    return score;
}

/**
 * @brief Performs tournament selection to select a parent for genetic algorithms.
 *
 * This function performs tournament selection to select a parent for genetic algorithms.
 * It randomly selects individuals from the population and compares their scores stored
 * in the `score` array. The individual with the best score is chosen as the parent.
 * The number of participants in each tournament is defined by `NB_PARTICIPATION_TOURNOI`.
 *
 * @param score An array of scores representing the fitness of each individual in the population.
 *
 * @return The index of the individual with the best score, representing the selected parent.
 */
int genetic_tournament_parent(int score[POPULATION_SIZE])
{
    int random_indice;
    int index_best_distance = rand() % POPULATION_SIZE;
    int iteration =0;
    
    do
    {
	random_indice = rand()%POPULATION_SIZE;
	
	if(score[random_indice] < score[index_best_distance])
	    index_best_distance = random_indice;
	++ iteration;
    } while(iteration < NB_PARTICIPATION_TOURNOI);

    return index_best_distance;
}

/**
 * @brief Generates a child through genetic crossover.
 *
 * This function generates a new child individual by performing genetic crossover
 * on two parent individuals from the population. The resulting child is stored in
 * the `individu` parameter.
 *
 * @param individu A pointer to the pointer of the individual that will hold the generated child.
 * @param population An array of pointers to individuals representing the population.
 * @param p1 The index of the first parent individual in the population.
 * @param p2 The index of the second parent individual in the population.
 *
 * @note The caller is responsible for allocating memory for the child individual and ensuring
 *       that the parent individuals are valid and within the range of the population array.
 */
void genetic_croisement_generate_child(rules_t children[NB_RULES], rules_t population[POPULATION_SIZE][NB_RULES], int p1, int p2)
{
    int i, k=0;
  
    for (i=0; i<NB_RULES/2 ; ++i)
    {
	rules_copy_rules(population[p1][i], &children[k]);
	++k;
    }

    for(i=NB_RULES/2; i<NB_RULES; ++i)
    {
	rules_copy_rules(population[p2][i] , &children[k]);
	++k;
    }
}


/**
 * @brief Applies genetic mutation to an individual.
 *
 * This function applies genetic mutation to an individual by modifying its genetic information.
 * The individual is represented by a pointer to a pointer to `rules_t`, whi
ch allows the function
 * to directly modify the individual.
 *
 * @param individu A pointer to the pointer of the individual to be mutated.
 *
 * @note The caller is responsible for ensuring that the individual is valid and properly allocated.
 *       The function will modify the individual in place.
 */
void genetic_mutate(rules_t individu[NB_RULES])
{
    int i;
    float p;
    float cum_p [NB_MEASURE +2];

    cum_p[0] = MUTATION_RATE_DISTANCE_FRIEND;
    cum_p[1] = cum_p[0] + MUTATION_RATE_DISTANCE_TARGET;
    cum_p[2] = cum_p[1] + MUTATION_RATE_CARDINALITY_FRIEND;
    cum_p[3] = cum_p[2] + MUTATION_RATE_CARDINALITY_TARGET;
    cum_p[4] = cum_p[3] + MUTATION_RATE_ACTION;
    cum_p[5] = cum_p[4] + MUTATION_RATE_PRIORITY;
    
    for (i = 0; i < NB_RULES; i++)
    {
        p = (float) rand() / (float) RAND_MAX;
        if ( p < cum_p[0])
	{
	    individu[i].perception.distance_friend = (rand()%NB_DISTANCE)-1;
        }
	else if (p < cum_p[1])
	{
	    individu[i].perception.distance_target = (rand()%NB_DISTANCE)-1;
        }
	else if (p < cum_p[2])
	{
	    individu[i].perception.cardinality_friend = (rand()%(NB_CARDINALITY-1))-1;
        }
	else if (p < cum_p[3])
	{
	    individu[i].perception.cardinality_target = (rand()%NB_CARDINALITY)-1 ;
        }
	else if  (p < cum_p[4])
	{
	    individu[i].action = rand()%NB_ACTION;
	}
	else if  (p < cum_p[5])
	{
	    individu[i].priority = rand()%NB_PRIORITY;
	}
    }
}


void genetic_solve_optimized(char * path_brain_load, char * path_best_brain) // game en parametre pour evavaluate population, puis individu 
{
    rules_t brain[NB_RULES];
    rules_read_path_file(path_brain_load, brain);
    genetic_solve_brain(brain, NULL);
    rules_save_path_file(path_best_brain, brain);
}


void genetic_solve_brain(rules_t brain[NB_RULES], int * score_best_brain) // game en parametre pour evavaluate population , puis individu 
{
    rules_t population     [POPULATION_SIZE][NB_RULES];
    rules_t new_population [POPULATION_SIZE][NB_RULES];
    int score[POPULATION_SIZE] = {0}; 
    
    int iteration = 0 , index_best_individu = 0, p1, p2;

    zlog(stdout, DEBUG, "Entrez dans genetic solve brain", NULL);

    genetic_initialize_population_brain(brain, population);
    genetic_initialize_population_brain(brain, new_population);
    
    while (iteration < MAX_ITERATIONS) { // MAX_ITERATIONS
	if(iteration%2 == 0) // population forme new_population
	{
	    index_best_individu = genetic_evaluate_population(score, population);
	    rules_copy_brain(population[index_best_individu], new_population[0]);

            // Sélection, croisement et mutation pour générer la nouvelle population
	    for (int i = 1; i < POPULATION_SIZE; i++)
	    {
		if(((float) rand()/ (float) RAND_MAX) > MUTATION_RATE)
		{	    
		    p1 = genetic_tournament_parent(score);
		    p2 = genetic_tournament_parent(score);
		    genetic_croisement_generate_child(new_population[i], population, p1, p2);

                    zlog(stdout, DEBUG, "p1: %d, p2: %d", p1, p2);
		}
		else
		{
		    rules_copy_brain_genetic(population[0], new_population[i]);
		    genetic_mutate(new_population[i]);
		}
	    }
	}
	else // new_populaiton forme population
	{
	    index_best_individu = genetic_evaluate_population(score, new_population);
	    rules_copy_brain(new_population[index_best_individu], population[0]);

	    // Sélection, croisement et mutation pour générer la nouvelle population
	    for (int i = 1; i < POPULATION_SIZE; i++)
	    {
		if(((float) rand()/ (float) RAND_MAX) > MUTATION_RATE)
		{
		    p1 = genetic_tournament_parent(score);
		    p2 = genetic_tournament_parent(score);

                    zlog(stdout, DEBUG, "p1: %d, p2: %d", p1, p2);

		    genetic_croisement_generate_child(population[i], new_population, p1, p2);
		}
		else
		{
		    rules_copy_brain(new_population[0], population[i]);
		    genetic_mutate(population[i]);
		}
	    }
	}
	++ iteration;
    }

    zlog(stdout, DEBUG, "best index : %d", index_best_individu);
    
    /* sauvegarde meillleur individu */
    (iteration%2==0)?
	rules_copy_brain(new_population[0], brain):
	rules_copy_brain(population[0], brain);

    *score_best_brain = score[0];
}
