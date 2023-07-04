#ifndef _simulation_h_
#define _simulation_h_

struct simulation_entity_s {
    int x;
    int y;
};

typedef simulation_entity_s simulation_entity_t;

void simulation_create(void);
void simulation_free(void);

#endif