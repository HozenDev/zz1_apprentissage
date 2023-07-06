#include "parallel.h"
#include "../log/log.h"
#include "../genetic/genetic.h"

struct thread_data_s {
    int iteration; /* nombre d'itération de la simulation */
    rules_t brain[NB_RULES]; /* cerveau de la simulation */
};

typedef struct thread_data_s thread_data_t;

int all_time;


/**
 * @brief Placeholder function for parallel treatment.
 *
 * This function serves as a placeholder for parallel treatment when an invalid function is provided. It accepts a `parameters`
 * input, but ignores it. The function logs an error message to the standard output indicating that it is not a valid function.
 * It always returns 0 to indicate successful execution.
 *
 * @param parameters  The input parameters for the parallel treatment (ignored).
 * @return            Always returns 0 to indicate successful execution.
 */
int parallel_treatment_not_a_function(void * parameters)
{
    (void) parameters;
    zlog(stdout, ERROR, "Ce n'est pas une fonction valide", NULL);
    return 0;
}


/**
 * @brief Perform parallel genetic treatment.
 *
 * This function performs parallel genetic treatment by solving the genetic algorithm on the specified brain in a separate thread.
 * The function expects a `thread_data_t` structure as the `parameters` input, containing the brain and iteration information.
 * The function measures the execution time of the genetic algorithm using the `clock` function and calculates the result and
 * elapsed time. The result is set to 0.0 since it is not explicitly calculated in the code. The elapsed time is calculated
 * in milliseconds using the CPU clock frequency. Finally, a log message is printed to the standard output indicating the
 * completion of the genetic solve and the elapsed time.
 *
 * @param parameters  The input parameters for the parallel treatment (including the brain and iteration information).
 * @return            Always returns 0 to indicate successful execution.
 */
int parallel_treatment_genetique(void * parameters)
{
    thread_data_t * tdata= (thread_data_t *) parameters;
    
    unsigned long millis;
    clock_t begin;
    clock_t end;
    float result = 0.0;

    zlog(stdout, INFO, "Threads launched", 0);
    begin = clock();
    genetic_solve_brain(tdata->brain, &tdata->iteration);
    end = clock();
    millis = (end -  begin) * 1000 / CLOCKS_PER_SEC;
    zlog(stdout, INFO, "GENETIC SOLVE: %f - Finished in %ld ms", result, millis);

    return 0;
}


/**
 * @brief Perform parallel simulated annealing treatment.
 *
 * This function serves as a placeholder for parallel simulated annealing treatment. It measures the execution time using the
 * `clock` function and calculates the result and elapsed time. The result is set to 0.0 since it is not explicitly calculated
 * in the code. The elapsed time is calculated in milliseconds using the CPU clock frequency. Finally, a log message is printed
 * to the standard output indicating the completion of the simulated annealing treatment and the elapsed time.
 *
 * @param parameters  The input parameters for the parallel treatment (ignored).
 * @return            Always returns 0 to indicate successful execution.
 */
int parallel_treatment_recuis_simule(void * parameters)
{    
    unsigned long millis;
    clock_t begin;
    clock_t end;
    float result = 0.0;

    (void) parameters;

    begin = clock();
    end = clock();
    millis = (end -  begin) * 1000 / CLOCKS_PER_SEC;
    zlog(stdout, INFO, "RECUIS SIMULE : %f - Finished in %ld ms", result, millis);

    return 0;
}

/**
 * @brief Perform parallel random greedy treatment.
 *
 * This function serves as a placeholder for parallel random greedy treatment. It measures the execution time using the `clock`
 * function and calculates the result and elapsed time. The result is set to 0.0 since it is not explicitly calculated in the
 * code. The elapsed time is calculated in milliseconds using the CPU clock frequency. Finally, a log message is printed to the
 * standard output indicating the completion of the random greedy treatment and the elapsed time.
 *
 * @param parameters  The input parameters for the parallel treatment (ignored).
 * @return            Always returns 0 to indicate successful execution.
 */
int parallel_treatment_glouton_aleatoire(void * parameters)
{
    unsigned long millis;
    clock_t begin;
    clock_t end;
    float result = 0.0;

    (void) parameters;
    
    begin = clock();
    end = clock();
    millis = (end -  begin) * 1000 / CLOCKS_PER_SEC;
    zlog(stdout, INFO, "GLOUTON ALEA : %f - Finished in %ld ms", result, millis);

    return 0;
}

/**
 * @brief Perform parallel loop for running different treatments.
 *
 * This function executes a parallel loop to run different treatments based on user input. It prompts the user to select a
 * function (0: random greedy, 1: genetic algorithm, 2: simulated annealing) and creates threads to execute the selected
 * function in parallel. The function waits for the threads to complete using `thrd_join` and checks the error codes of each
 * thread. After each iteration, it prompts the user to continue or exit the loop. The loop continues until the user chooses to
 * exit by entering 0.
 *
 * @return  Always returns 0 to indicate successful execution.
 */
int parallel_loop(void)
{
    int i, running, i_function;
    
    thrd_t thread_handle[NB_THREADS];
    int error_code_of_thread[NB_THREADS] = {0};
    int (*fres) (void *) = NULL;

    running = 1;
    
    while (running)
    {
        fprintf(stdout, "\nQuelle fonction ? [0: GA, 1: G, 2: RS] ");
        fscanf(stdin, "%d", &i_function);
        fprintf(stdout, "\n");
        
        switch (i_function)
        {
        case 0: fres = parallel_treatment_glouton_aleatoire; break;
        case 1: fres = parallel_treatment_genetique; break;
        case 2: fres = parallel_treatment_recuis_simule; break;
        default: fres = parallel_treatment_not_a_function; break;
        }
        
        for (i = 0; i < NB_THREADS; ++i) thrd_create(&thread_handle[i], *fres, NULL);
        for (i = 0; i < NB_THREADS; ++i) thrd_join(thread_handle[i], &error_code_of_thread[i]);

        fprintf(stdout, "\nContinuer ? [0: non, 1: oui] ");
        if (fscanf(stdin, "%d", &running) < 1)
            running = 0;
        fprintf(stdout, "\n");
    }

    return 0;
}


/**
 * @brief Perform multiple simulations in parallel and determine the minimum iteration and average iteration.
 *
 * This function performs multiple simulations in parallel using the genetic algorithm treatment. It creates threads to
 * execute the simulations in parallel, each with its own copy of the brain rules. After the simulations are completed, it
 * determines the minimum iteration and calculates the average iteration across all simulations. The minimum iteration is
 * stored in the `min` variable, and the average iteration is stored in the `mean` variable. The function also copies the
 * brain of the simulation with the minimum iteration back to the original brain. Finally, it logs the minimum iteration and
 * average iteration to the standard output.
 *
 * @param brain          The brain rules to be used in the simulations.
 * @param iter_average   A pointer to an integer to store the average iteration.
 */
void parallel_multiple_simulation(rules_t brain[NB_RULES], int * iter_average)
{
    thrd_t threads[NB_THREADS];
    thread_data_t tdata[NB_THREADS];
    int error_code_of_thread[NB_THREADS];
    int min = ITER_MAX;
    float mean = 0.0;
    int i, i_min = 0;
    int (*fres) (void *) = NULL;

    fres = parallel_treatment_genetique;

    zlog(stdout, INFO, "Simulation parallele lancé sur %d threads", NB_THREADS);
    
    for (i = 0; i < NB_THREADS; ++i)
    {
        rules_copy_brain(brain, tdata[i].brain);
        thrd_create(&threads[i], *fres, (void *)&tdata[i]);   
    }
    for (i = 0; i < NB_THREADS; ++i) thrd_join(threads[i], &error_code_of_thread[i]);

    for (i = 0; i < NB_THREADS; ++i) {
        if (tdata[i].iteration < min) {
            min = tdata[i].iteration;
            i_min = i;
        }
        mean += tdata[i].iteration;
    }

    mean /= NB_THREADS;
    zlog(stdout, INFO, "Itération minimum: %d - Moyenne des itérations: %f", min, mean);
    *iter_average = mean;

    rules_copy_brain(tdata[i_min].brain, brain);
}
