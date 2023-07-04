#include "simulation.h"
#include <math.h>
#include <limits.h>

#include <stdlib.h>

simulation_target_t target;

void simulation_create(void)
{
    /* todo */
}

void simulation_free(void)
{
    /* todo */
}

/* ------- ACTIONS -------- */

void simulation_destroy_target(simulation_entity_t predator)
{
    if (predator.p.distance_target == CLOSE) {
        target.pv -= PREDATOR_DAMAGE;
    }
}

void simulation_communicate(simulation_entity_t predator, simulation_entity_t predators[NB_PREDATOR])
{
    int i;

    if (predator.p.cardinality_target != NOT_FOUND)
    {
        for (i = 0; i < NB_PREDATOR; ++i)
        {
            if (simulation_get_distance_between_2_predator(predator, predators[i]) < COM_RADIUS)
            {
                predators[i].p.cardinality_target = predator.p.cardinality_target;
            }
        }
    }
}

void simulation_move_entity(simulation_entity_t predator, enum cardinality c)
{
    switch (c)
    {
    case NORTH:
        predator.y -= PREDATOR_SPEED;
        if (predator.y < 0) predator.y = 0;
        break;
    case SOUTH:
        predator.y += PREDATOR_SPEED;
        if (predator.y > WORLD_HEIGHT) predator.y = WORLD_HEIGHT;
        break;
    case EAST:
        predator.x += PREDATOR_SPEED;
        if (predator.x > WORLD_WIDTH) predator.y = WORLD_WIDTH;
        break;
    case WEST:
        predator.x -= PREDATOR_SPEED;
        if (predator.y < 0) predator.y = 0;
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
    if(deltay>0 && delta>0) card=NORTH;

    return(card);
}

void simulation_get_closest_friend(simulation_entity_t * predators)
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

void simulation_get_perception(simulation_entity_t * predators)
{
    int i;
    
    simulation_get_closest_friend(predators);
    for(i=0;i<NB_PREDATOR;i++)
    {
        if(predators[i].p.cardinality_target != NOT_FOUND || 
            simulation_get_distance(abs(predators[i].x - target.x) + abs(predators[i].y - target.y))){
            predators[i].p.cardinality_target =
                simulation_get_cardinals(predators[i].x,target.x, predators[i].y,target.y);
            predators[i].p.distance_target =
                simulation_get_distance(abs(predators[i].x - target.x) + abs(predators[i].y - target.y));
        }
    }
}

int simulation_get_distance_between_2_predator(simulation_entity_t p1, simulation_entity_t p2)
{
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

/* ------- REGLES --------- */

int simulation_verify_rules(simulation_entity_t predators,rules_t rule)
{
    int flag=1;
    /*test distance friend*/
    if(predators.p.distance_friend!=JOKER_D
       && predators.p.distance_friend==rule.perception.distance_friend) flag=0;
   
    /*test distance target*/
    if(predators.p.distance_target != JOKER_D
       && predators.p.distance_target==rule.perception.distance_target) flag=0;
    
    /*test cardinality friend*/
    if(predators.p.cardinality_friend!=JOKER_C
       && predators.p.cardinality_friend==rule.perception.cardinality_friend) flag=0;
    
    /*test direction target*/
    if(predators.p.cardinality_target != JOKER_C
       && predators.p.cardinality_target == rule.perception.cardinality_target) flag=0;

    return(flag);
}

void simulation_filtrage_regle(simulation_entity_t predators, int filtered_rules[NB_RULES], rules_t brain[NB_RULES])
{
    int i;
    for(i=0;i<NB_RULES;i++){
        if (simulation_verify_rules(predators,brain[i]))
        {
            filtered_rules[i] = 1;
        }
    }
}

int simulation_choose_action(int filtered_rules[NB_RULES], rules_t * brain)
{
    int sum=0;
    float cumulativeProbability=0;
    int action=0;
    int j;

    int probability[NB_RULES]={0};
    
    for(j=0;j<NB_RULES;j++)
    {
        if(filtered_rules[j] == 1)
        {
            sum += pow(brain[j].priority, S_POWER);
            probability[j] = pow(brain[j].priority, S_POWER);
        }
    }

    for(j=0;j<NB_RULES;j++){
        if(filtered_rules[j]==1){
            cumulativeProbability+=probability[j]/sum;
            if(rand()/RAND_MAX<cumulativeProbability){
                action=j;
            }
            
        }
    }
    return(action);
}

void simulation_execute_action(simulation_entity_t predator,
                               int action,
                               simulation_entity_t predators[NB_PREDATOR])
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
        simulation_communicate(predator, predators);
        break;
    case 5:
        simulation_destroy_target(predator);
        break;
    }
}
void simulation_init(simulation_entity_t predators[NB_PREDATOR])
{
    int i;
    
    /* target.x=(float)rand()/RAND_MAX*WORLD_WIDTH; */
    /* target.y=(float)rand()/RAND_MAX*WORLD_HEIGHT; */
    target.x = 0;
    target.y = 0;
    target.pv=TARGET_PV;

    for(i=0;i<NB_PREDATOR;i++)
    {
        predators[i].x=(float)rand()/RAND_MAX*WORLD_WIDTH;
        predators[i].y=(float)rand()/RAND_MAX*WORLD_HEIGHT;
    }
    predators[0].x = 0;
    predators[0].y = 0;
}   
void simulation_loop(rules_t brain[NB_RULES], int * iter)
{
    int action[NB_RULES]={0};
    int filtered_rules[NB_RULES];
    simulation_entity_t predators[NB_PREDATOR];
    int i, j;

    *iter=0;
    simulation_init(predators);
    
    while(target.pv > 0 && *iter < ITER_MAX)
    {
        (*iter) ++;
        simulation_get_perception(predators);
        for(i=0;i<NB_PREDATOR;i++) printf("%d",predator.distance_target)
        
        for(i=0;i<NB_PREDATOR;i++)
        {
            /*init filtered_rules*/
            for(j=0;j<NB_RULES;j++) filtered_rules[j]=0;
            /* filter rules */
            simulation_filtrage_regle(predators[i], filtered_rules, brain);
            /* choisis une action */
            action[i]=simulation_choose_action(filtered_rules,brain);
        }

        /* execute action */
        for(i=0;i<NB_PREDATOR;i++) simulation_execute_action(predators[i], action[i], predators);
    }
}
