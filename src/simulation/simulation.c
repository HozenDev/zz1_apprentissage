#include "simulation.h"
#include <math.h>
#include <limits.h>

#include <stdlib.h>
#include "../utils/utils.h"
void simulation_create(void)
{
    /* todo */
}

void simulation_free(void)
{
    /* todo */
}

/* ------- ACTIONS -------- */

void simulation_destroy_target(entity_t predator, target_t * target)
{
    if (predator.p.distance_target == CLOSE) {
        target->pv -= PREDATOR_DAMAGE;
    }
}

void simulation_communicate(entity_t predator, entity_t predators[NB_PREDATOR])
{
    int i;

    if (predator.p.cardinality_target != NOT_FOUND)
    {
        for (i = 0; i < NB_PREDATOR; ++i)
        {
            if (simulation_get_distance_between_2_predator(predator, predators[i]) < 0.5*COM_RADIUS)
            {
                predators[i].p.cardinality_target = predator.p.cardinality_target;
            }
        }
    }
}

void simulation_move_entity(entity_t * predator, enum cardinality c)
{
    switch (c)
    {
    case NORTH:
        predator->y -= PREDATOR_SPEED;
        if (predator->y < 0) predator->y = 0;
        break;
    case SOUTH:
        predator->y += PREDATOR_SPEED;
        if (predator->y > WORLD_HEIGHT) predator->y = WORLD_HEIGHT;
        break;
    case EAST:
        predator->x += PREDATOR_SPEED;
        if (predator->x > WORLD_WIDTH) predator->x = WORLD_WIDTH;
        break;
    case WEST:
        predator->x -= PREDATOR_SPEED;
        if (predator->x < 0) predator->x = 0;
        break;
    case NOT_FOUND:
        /* to do */
        break;
    case JOKER_C:
        /* todo */
        break;
    }
}

/* ------- PERCEPTIONS ---------- */

enum cardinality simulation_get_cardinals(float xa,float ya,float xb ,float yb)
{
    enum cardinality card;
    int 
        deltax=xa - xb,
        deltay=ya - yb,
        delta=abs(deltax) - abs(deltay);

    if(deltax>0 && delta>0) card=WEST;
    if(deltax<0 && delta>0) card=EAST;
    if(deltay<0 && delta<0) card=SOUTH;
    if(deltay>0 && delta<0) card=NORTH;

    return(card);
}

void simulation_get_closest_friend(entity_t * predators)
{
    int distmin = INT_MAX;
    int dist = 0;
    int friend = 0;
    int i, j;

    for (i=0;i<NB_PREDATOR;i++){
        for(j=0;j<NB_PREDATOR;j++)
        {
            if(i!=j)
            {
                dist = simulation_get_distance_between_2_predator(predators[i], predators[j]);
                if(dist<distmin)
                {
                    dist=distmin;
                    friend=j;
                }
            }
        }
        predators[i].p.cardinality_friend =
            simulation_get_cardinals(predators[i].x,predators[friend].x,predators[i].y,predators[friend].y);
        predators[i].p.distance_friend =
            simulation_get_distance(distmin);
    }
}


enum distance simulation_get_distance(float dsrc)
{
    enum distance dist;
    if (dsrc <= COM_RADIUS) dist=CLOSE;
    else dist=FAR;
    return dist;
}

void simulation_get_perception(entity_t * predators, target_t target)
{
    int i;
    
    simulation_get_closest_friend(predators);
    for(i=0;i<NB_PREDATOR;i++)
    {
        if(predators[i].p.cardinality_target != NOT_FOUND
           || simulation_get_distance(abs(predators[i].x - target.x) + abs(predators[i].y - target.y)) == CLOSE)
        {
            predators[i].p.cardinality_target =
                simulation_get_cardinals(predators[i].x,target.x, predators[i].y,target.y);
            predators[i].p.distance_target =
                simulation_get_distance(abs(predators[i].x - target.x) + abs(predators[i].y - target.y));
        }
    }
}

int simulation_get_distance_between_2_predator(entity_t p1, entity_t p2)
{
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

/* ------- REGLES --------- */

int simulation_verify_rules(entity_t predators,rules_t rule)
{
    int flag=1;
    /*test distance friend*/
    if(rule.perception.distance_friend != JOKER_D
       && predators.p.distance_friend != rule.perception.distance_friend) flag=0;
   
    /*test distance target*/
    if(rule.perception.distance_target!=JOKER_D
       && predators.p.distance_target != rule.perception.distance_target) flag=0;
    
    /*test cardinality friend*/
    if(rule.perception.cardinality_friend!=JOKER_C
       && predators.p.cardinality_friend != rule.perception.cardinality_friend) flag=0;
    
    /*test direction target*/
    if(rule.perception.cardinality_target != JOKER_C
       && predators.p.cardinality_target != rule.perception.cardinality_target) flag=0;

    return(flag);
}

int simulation_filtrage_regle(entity_t predators, int filtered_rules[NB_RULES], rules_t brain[NB_RULES])
{
    int i;
    int nb_compatible=0;
    for(i=0;i<NB_RULES;i++){
        if (simulation_verify_rules(predators,brain[i]))
        {
            filtered_rules[nb_compatible] = i;
            nb_compatible++;
        }
    }
    return(nb_compatible);
}

int simulation_choose_action(int filtered_rules[NB_RULES], rules_t * brain,int nb_compatible)
{
    float sum=0.0;
    float cumulativeProbability=0.0;
    int action=0;
    int j;
    float p=(float) rand()/(RAND_MAX) ;

    float probability[NB_RULES]={0};
    utils_shuffle(filtered_rules,nb_compatible);
    for(j=0;j<nb_compatible;j++)
    {
        sum += powf(brain[filtered_rules[j]].priority, S_POWER);
        probability[filtered_rules[j]] = powf(brain[filtered_rules[j]].priority, S_POWER);
    }
    
    for(j=0;j<nb_compatible;j++) {
            cumulativeProbability += probability[filtered_rules[j]]/sum;
            if(p< cumulativeProbability){
                action = brain[filtered_rules[j]].action;
                break;
            }
    }
    return(action);
}

void simulation_execute_action(entity_t * predator,
                               int action,
                               entity_t predators[NB_PREDATOR],
                               target_t * target)
{
    switch(action)
    {
    case 0:
        simulation_move_entity(predator,NORTH);
        break;
    case 1:
        simulation_move_entity(predator,SOUTH);
        break;
    case 2:
        simulation_move_entity(predator,EAST);
        break;
    case 3:
        simulation_move_entity(predator,WEST);
        break;
    case 4:
        simulation_communicate(*predator, predators);
        break;
    case 5:
        simulation_destroy_target(*predator, target);
        break;
    }
}
void simulation_init(entity_t predators[NB_PREDATOR], target_t * target)
{
    int i;
    entity_initialize_target(target);
    for(i=0;i<NB_PREDATOR;i++)
        entity_initialize(&predators[i]);
}

void simulation_loop(rules_t brain[NB_RULES], int * iter)
{
    
    int action[NB_RULES]={0};
    int filtered_rules[NB_RULES] = {0};
    entity_t predators[NB_PREDATOR];
    target_t target;
    int i, j;
    int nb_compatible=0;
    printf("je suis rentré");
    *iter=0;
    simulation_init(predators, &target);

    while(target.pv > 0 && *iter < ITER_MAX)
    {
        (*iter) ++;
        simulation_get_perception(predators, target);
        
        
        for(i=0;i<NB_PREDATOR;i++)
        {
            /*init filtered_rules*/
            for(j=0;j<NB_RULES;j++) filtered_rules[j]=0;
            /* filter rules */
            nb_compatible=simulation_filtrage_regle(predators[i], filtered_rules, brain);
            /* choisis une action */
            action[i] = simulation_choose_action(filtered_rules,brain,nb_compatible);
            
        }
        /* execute action */
        for(i=0;i<NB_PREDATOR;i++) simulation_execute_action(&predators[i], action[i], predators, &target);
    }
}
