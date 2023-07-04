#ifndef _rules_h_
#define _rules_h_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../log/log.h"
#include "../const/const.h"

enum distance {
    JOKER_D = -1,
    CLOSE,
    FAR
};

enum cardinality {
    JOKER_C = -1,
    NORTH,
    SOUTH,
    EAST,
    WEST,
    NOT_FOUND
};

struct perception_s {
    enum distance distance_friend;
    enum cardinality cardinality_friend;
    enum distance distance_target;
    enum cardinality cardinality_target;

};
typedef struct perception_s perception_t;

struct rules_s {
    perception_t perception;
    int action;
    int priority;    
};

typedef struct rules_s rules_t;  


void rules_save_file(FILE * file, rules_t array_rules[NB_RULES]);
void rules_save_path_file(char * path_file, rules_t array_rules[NB_RULES]);

void rules_read_file(FILE * file, rules_t array_rules[NB_RULES]);
void rules_read_path_file(char* path_file, rules_t array_rules[NB_RULES]);

void rules_copy_brain(rules_t brainsrc[NB_RULES], rules_t braindest[NB_RULES]);
void rules_copy_brain_genetic(rules_t brainsrc[NB_RULES], rules_t braindest[NB_RULES]);
void rules_copy_rules(rules_t rules_src, rules_t * rules_dest);

#endif
