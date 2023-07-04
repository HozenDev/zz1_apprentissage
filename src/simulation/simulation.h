#ifndef _simulation_h_
#define _simulation_h_

#include "../const/const.h"

#define SIMULATION_WORLD_WIDTH SCREEN_WIDTH
#define SIMULATION_WORLD_HEIGHT SCREEN_HEIGHT

struct simulation_entity_s {
    int x;
    int y;
};
enum distance{
    close,
    far
};
enum direction_friend{
    north,
    south,
    east,
    west
};
enum direction_target{
    not_found,
    north,
    south,
    east,
    west,

};
typedef simulation_entity_s simulation_entity_t;

extern struct simulation_entity_s target;

void simulation_create(void);
void simulation_free(void);

#endif
