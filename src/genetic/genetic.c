#include "genetic.h"


// chaque individu est un tableau de règle



/**
 * @brief Creates a new individual for genetic algorithms.
 *
 * This function creates a new individual for genetic algorithms by allocating memory
 * for a `rules_t` structure and initializing it using the `rules_create_array_rules`
 * function.
 *
 * @return A pointer to the newly created individual.
 *
 * @note The caller is responsible for freeing the memory allocated for the individual.
 */
rules_t * genetic_create_individu()
{
    rules_t * individu;
    /* rules_create_array_rules(&individu); */
    return individu;
}


/**
 * @brief Destroys an individual created for genetic algorithms.
 *
 * This function destroys an individual created for genetic algorithms by freeing
 * the memory allocated for the `rules_t` structure using the `rules_destroy_array_rules`
 * function.
 *
 * @param individu A pointer to the individual to be destroyed.
 *
 * @note The function assumes that the individual was previously allocated using the
 *       `genetic_create_individu` function.
 */
void genetic_destroy_individu(rules_t * individu)
{
    (void) individu;
    /* rules_destroy_array_rules(individu); */
}


/**
 * @brief Initializes the population for genetic algorithms.
 *
 * This function initializes the population for genetic algorithms by creating and
 * populating an array of individuals. Each individual is created using the
 * `genetic_create_individu` function.
 *
 * @return A pointer to the array of initialized individuals.
 *
 * @note The population array is a local static array, so it will remain valid after
 *       the function returns. However, be cautious when using this approach if the
 *       population size is large, as it may lead to stack overflow.
 */
rules_t **genetic_initialize_population()
{
    rules_t ** population = malloc(POPULATION_SIZE * sizeof(rules_t*));

    for (int i = 0; i < POPULATION_SIZE; ++i)
    {
        population[i] = genetic_create_individu();
    }
    return population;
}

/**
 * @brief Destroys the population created for genetic algorithms.
 *
 * This function destroys the population created for genetic algorithms by
 * freeing the memory allocated for each individual in the static array.
 *
 * @param population A pointer to the array of individuals to be destroyed.
 *
 * @note The function assumes that the population was previously initialized using
 *       the `genetic_initialize_population` function. However, since a static array
 *       was used, the memory for the individuals will not be freed automatically.
 *       Therefore, it's important to call this function to avoid memory leaks.
 */
void genetic_destroy_population(rules_t **population)
{
    for (int i = 0; i < POPULATION_SIZE; ++i)
    {
        genetic_destroy_individu(population[i]);
    }
    free(population);
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
int genetic_evaluate_population(int score[POPULATION_SIZE],rules_t ** population)
{
    int best_individu = INT_MAX;
    for (int i=0; i < POPULATION_SIZE; ++i)
    {
	score[i] = genetic_evaluate_individu(population[i]);
	if (score[i] > best_individu) best_individu = i;
    }
    return best_individu;
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
    (void) individu;
    // TO DO
    return 0;
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
int genetic_tournament_parent(int * score)
{
    int index_best_distance = INT_MAX;
    int iteration =0;
    int random_indice;
    
    do
    {
	random_indice = rand()%POPULATION_SIZE;
	if(score[random_indice] < index_best_distance) index_best_distance = score[random_indice];
	
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
void genetic_croisement_generate_child(rules_t ** children, rules_t ** population, int p1, int p2)
{
    // TO DO
    (void) children;
    (void) population;
    (void) p1;
    (void) p2;
}


/**
 * @brief Applies genetic mutation to an individual.
 *
 * This function applies genetic mutation to an individual by modifying its genetic information.
 * The individual is represented by a pointer to a pointer to `rules_t`, which allows the function
 * to directly modify the individual.
 *
 * @param individu A pointer to the pointer of the individual to be mutated.
 *
 * @note The caller is responsible for ensuring that the individual is valid and properly allocated.
 *       The function will modify the individual in place.
 */
void genetic_mutate(rules_t ** individu)
{
    (void) individu;
    // TO DO
}


rules_t * genetic_solve_optimized() // game en parametre pour evavaluate population , puis individu 
{
    rules_t ** population;
    rules_t ** new_population;

    int score[POPULATION_SIZE]; // score de chaque individu de la population parent
    
    unsigned int iteration = 0, index_best_individu, p1, p2;

    rules_t * best_individu;
    best_individu = (rules_t *) malloc(sizeof(rules_t));
    
    population = genetic_initialize_population();
    
    new_population = genetic_initialize_population();

    while (iteration < MAX_ITERATIONS) { // MAX_ITERATIONS

	if(iteration%2 == 0) // population forme new_population
	{
	    //calcule distance
	    for (int i = 0; i < POPULATION_SIZE; i++)
	    {
		index_best_individu = genetic_evaluate_population(score, population);
	    }

	    rules_copy_brain(population[index_best_individu], new_population[0]);
	    // Sélection, croisement et mutation pour générer la nouvelle population
	    for (int i = 1; i < POPULATION_SIZE; i++)
	    {
		p1 = genetic_tournament_parent(score);
		p2 = genetic_tournament_parent(score);
		
		genetic_croisement_generate_child(&new_population[i], population, p1, p2);

		//génère une mutation
		if((float) (rand() / RAND_MAX) < MUTATION_RATE) genetic_mutate(&new_population[i]);
	    }
	}
	else // new_populaiton forme population
	{
	    //calcule distance
	    for (int i = 0; i < POPULATION_SIZE; i++)
	    {
		index_best_individu = genetic_evaluate_population(score, new_population);
	    }
	    
	    rules_copy_brain(population[index_best_individu], population[0]);
	    // Sélection, croisement et mutation pour générer la nouvelle population
	    for (int i = 1; i < POPULATION_SIZE; i++)
	    {
		p1 = genetic_tournament_parent(score);
		p2 = genetic_tournament_parent(score);
		
	        genetic_croisement_generate_child(&population[i], new_population, p1, p2);

		//génère une mutation
		if((float) (rand() / RAND_MAX) < MUTATION_RATE) genetic_mutate(&population[i]);
	    }
	}
	++ iteration;
    }

    (iteration%2==0)? rules_copy_brain(population[0], best_individu) : rules_copy_brain(new_population[0], best_individu);
    
    
    genetic_destroy_population(population);
    genetic_destroy_population(new_population);
    
    return best_individu;
}
