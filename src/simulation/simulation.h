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

extern int target_x;
extern int target_y;

void simulation_create(void);
void simulation_free(void);

#endif
