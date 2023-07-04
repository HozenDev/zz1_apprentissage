#ifndef _simulation_h_
#define _simulation_h_

#include "../const/const.h"
#include "../rules/rules.h"

struct simulation_entity_s {
    int x;
    int y;
    struct perception_s p;
};

typedef simulation_entity_s simulation_entity_t;

struct simulation_target_s {
    int target_x;
    int target_y;
    int pv;
};

typedef simulation_target_s simulation_target_t;

extern simulation_target_s target;

void simulation_create(void);
void simulation_free(void);

#endif
