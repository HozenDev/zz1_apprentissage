#include "parallel.h"
#include "../log/log.h"
#include "../genetic/genetic.h"

#define NB_THREADS 10

struct thread_data_s {
    int iteration; /* nombre d'itération de la simulation */
    rules_t brain[NB_RULES]; /* cerveau de la simulation */
};

typedef struct thread_data_s thread_data_t;

int all_time;

int parallel_treatment_not_a_function(void * parameters)
{
    (void) parameters;
    zlog(stdout, ERROR, "Ce n'est pas une fonction valide", NULL);
    return 0;
}

int parallel_treatment_genetique(void * parameters)
{
    thread_data_t * tdata= (thread_data_t *) parameters;
    
    unsigned long millis;
    clock_t begin;
    clock_t end;
    float result = 0.0;
    
    begin = clock();
    /* genetic_solve_brain(tdata->brain, tdata->iteration); */
    tdata->iteration = rand()%10;
    end = clock();
    millis = (end -  begin) * 1000 / CLOCKS_PER_SEC;
    zlog(stdout, INFO, "GENETIC SOLVE: %f - Finished in %ld ms", result, millis);

    return 0;
}

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

void parallel_multiple_simulation(rules_t brain[NB_RULES], int * iter_average)
{
    thrd_t threads[NB_THREADS];
    thread_data_t tdata[NB_THREADS];
    int error_code_of_thread[NB_THREADS];
    int min = ITER_MAX;
    float mean = 0.0;
    int i;
    int (*fres) (void *) = NULL;

    fres = parallel_treatment_genetique;
    
    for (i = 0; i < NB_THREADS; ++i) thrd_create(&threads[i], *fres, (void *)&tdata[i]);
    for (i = 0; i < NB_THREADS; ++i) thrd_join(threads[i], &error_code_of_thread[i]);

    for (i = 0; i < NB_THREADS; ++i) {
        if (tdata[i].iteration < min)
            min = tdata[i].iteration;
        mean += tdata[i].iteration;
    }
    mean /= NB_THREADS;
    zlog(stdout, INFO, "Itération minimum: %d - Moyenne des itérations: %f", min, mean);
}
