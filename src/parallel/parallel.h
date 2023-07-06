#ifndef _parallel_h_
#define _parallel_h_

#include <threads.h>
#include <stdio.h>
#include <time.h>
#include "../rules/rules.h"

#define NB_THREADS 8

int parallel_loop(void);
void parallel_multiple_simulation(rules_t brain[NB_RULES], int * iter_average);

#endif
