#ifndef _simulation_h_
#define _simulation_h_

#include "../const/const.h"
#include "../rules/rules.h"

struct simulation_entity_s {
    int x;
    int y;
    struct perception_s p;
};

typedef struct simulation_entity_s simulation_entity_t;

struct simulation_target_s {
    int x;
    int y;
    int pv;
};

typedef struct simulation_target_s simulation_target_t;

extern struct simulation_target_s target;

void simulation_create(void);
void simulation_free(void);

#endif
