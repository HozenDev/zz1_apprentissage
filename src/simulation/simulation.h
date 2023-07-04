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

void simulation_destroy_target(simulation_entity_t predator);
void simulation_communicate(simulation_entity_t predator, simulation_entity_t predators[NB_PREDATOR]);
void simulation_move_entity(simulation_entity_t * predator, enum cardinality c);

enum cardinality simulation_get_cardinals(float xa,float ya,float xb ,float yb);
void simulation_get_closest_friend(simulation_entity_t * predators);
enum distance simulation_get_distance(float dsrc);
void simulation_get_perception(simulation_entity_t * predators);
int simulation_get_distance_between_2_predator(simulation_entity_t p1, simulation_entity_t p2);

int simulation_verify_rules(simulation_entity_t predators, rules_t rule);
int simulation_choose_action(int filtered_rules[NB_RULES], rules_t * brain);
void simulation_execute_action(simulation_entity_t * predator,
                               int action,
                               simulation_entity_t predators[NB_PREDATOR]);

void simulation_create(void);
void simulation_free(void);
void simulation_loop();
void simulation_init(simulation_entity_t predators[NB_PREDATOR]);

#endif
