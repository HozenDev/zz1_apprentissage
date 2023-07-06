#include "seed.h"

/**
 * @brief Save a seed to a file.
 *
 * This function saves the specified seed to a file named "seed_save".
 *
 * @param seed   The seed to be saved.
 */
void save_seed(long int seed)
{
    FILE *fp = fopen("seed_save", "w+");
    fprintf(fp, "%ld", seed);
    fclose(fp);
}


/**
 * @brief Generate a seed based on the current time.
 *
 * This function generates a seed based on the current time and stores it in the specified variable.
 *
 * @param seed   Pointer to a long integer variable to store the generated seed.
 */
void create_seed(long int * seed)
{
    *seed = time(NULL);
}


/**
 * @brief Load a saved seed from a file.
 *
 * This function loads a saved seed from a file and stores it in the specified variable.
 *
 * @param seed   Pointer to a long integer variable to store the loaded seed.
 */
void load_seed(long int * seed)
{
    FILE *fp = fopen("seed_save", "r+");
    fscanf(fp, "%ld", seed);
    fclose(fp);
}

/**
 * @brief Generate a seed for random number generation.
 *
 * This function generates a seed for random number generation based on the specified behavior.
 *
 * @param behavior   The behavior for generating the seed. Use 0 to generate a random seed and save it, 1 to load a saved seed, or any other value to generate a random seed and save it.
 */
void generate_seed(int behavior)
{
    long int seed;
    create_seed(&seed);
    switch (behavior) {
    case 0:
	save_seed(seed);
	break;
    case 1:
	load_seed(&seed);
	break;
    default:
	fprintf(stdout, "generate_seed(int behaviour)\tparameter invalid: behaviour (0 or 1)\n");
	fprintf(stdout, "\t=> random seed and save\n");
	save_seed(seed);
	break;
    }
    srand(seed);
}
