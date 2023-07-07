#ifndef _simulation_h_
#define _simulation_h_

#include "../const/const.h"
#include "../rules/rules.h"
#include "../entity/entity.h"

void simulation_destroy_target(entity_t * predator, target_t targets[NB_PREY]);
void simulation_communicate(entity_t predator, entity_t predators[NB_PREDATOR]);
void simulation_move_entity(entity_t * predator, enum cardinality c);

enum cardinality simulation_get_cardinals(float xa,float ya,float xb ,float yb);
void simulation_get_closest_friend(entity_t * predators);
enum distance simulation_get_distance(int dsrc, int radius);
void simulation_get_perception(entity_t predators[NB_PREDATOR], target_t target[NB_PREY]);
int simulation_get_distance_between_2_predator(entity_t p1, entity_t p2);
int simulation_get_distance_predator_target(entity_t p, target_t t);

int simulation_verify_rules(entity_t predators, rules_t rule);
int simulation_choose_action(int  filtered_rules[NB_RULES], rules_t brain[NB_RULES],int nb_compatible);
void simulation_execute_action(entity_t * predator,
                               int action,
                               entity_t predators[NB_PREDATOR],
                               target_t targets[NB_PREY]);
int simulation_filtrage_regle(entity_t predators, int filtered_rules[NB_RULES], rules_t brain[NB_RULES]);

void simulation_create(void);
void simulation_free(void);
void simulation_loop(rules_t brain[NB_RULES], int * iter);
void simulation_loop_average(rules_t brain[NB_RULES], int * iter);
void simulation_init(entity_t predators[NB_PREDATOR], target_t target[NB_PREY]);

#endif
