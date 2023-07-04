#ifndef _simulation_h_
#define _simulation_h_

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

void simulation_create(void);
void simulation_free(void);

#endif
