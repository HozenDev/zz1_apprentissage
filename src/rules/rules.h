#ifndef _rules_h_
#define _rules_h_

#define NB_MEASURE 3   // size of measures
#define NB_RULES   10  // number rules in brain (size of array_rules)


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../log/log.h"


struct rules_s {
    char measures[NB_MEASURE]; 
    char action;
    char priority;    
};

typedef struct rules_s rules_t;  


void rules_save_file(FILE * file, rules_t * array_rules);
void rules_save_path_file(char * path_file, rules_t * array_rules);

void rules_read_file(FILE * file, rules_t* array_rules);
void rules_read_path_file(char* path_file, rules_t* array_rules);

void rules_copy_brain(rules_t brain_src[NB_RULES], rules_t brain_dest[NB_RULES]);
void rules_copy_rules(rules_t rules_src, rules_t rules_dest);

void rules_create_array_rules(rules_t ** array_rules);
void rules_destroy_array_rules(rules_t * array_rules);

#endif
