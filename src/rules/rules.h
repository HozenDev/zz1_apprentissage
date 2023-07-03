#ifndef _rules_h_
#define _rules_h_

#define NB_MEASURE 3// ?
#define NB_RULES 10

#include <stdio.h>
#include <stdlib.h>

struct rules_s {
    char measures[NB_MEASURE]; 
    char action;
    char priority;    
};

typedef struct rules_s rules_t;  


void rules_save_file(FILE * file, char * path_file, rules_t * array_rules);

void rules_init_file(FILE * file, char * path_file, rules_t * array_rules);

void rules_copy_brain(rules_t * rulesscr,rules_t * rulesdest);


#endif
