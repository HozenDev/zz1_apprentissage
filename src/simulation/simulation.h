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
void simulation_loop();
void simulation_execute_action(simulation_entity_t predator,int action,simulation_entity_t * predators);
int simulation_choose_action(int * filtered_rules,rules_t * brain);
void simulation_filtrage_regle(simulation_entity_t predators,rules_t * brain,int * filtered_rules[NB_RULES]);
int simulation_verify_rules(simulation_entity_t predators,rules_t rule);
int simulation_get_distance_between_2_predator(simulation_entity_t p2, simulation_entity_t p2);
void simulation_get_perception(simulation_entity_t * predators);
enum direction_friend simulation_get_cardinals(float xa,float ya,float xb ,float yb);
enum distance simulation_get_distance(float distance);
void simulation_get_closest_friend(simulation_entity_t * predators);
void simulation_move_entity(simulation_entity_t predator, enum cardinality c);
int simulation_communicate(simulation_entity_t predator, simulation_entity_t * predators[NB_PREDATOR]);
void simulation_destroy_target(simulation_entity_t predator);

#endif
