#ifndef _resolution_h_
#define _resolution_h_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include  "../rules/rules.h"
#include  "../const/const.h"
#include  "../simulation/simulation.h"
#include "../seed/seed.h"
#include "../utils/utils.h"

void resolution_recuis_simule(float (*pf)(float), char * path_brain_load, char * path_brain_res, int * score);
void resolution_random_change(rules_t brain[NB_RULES]);

#endif
