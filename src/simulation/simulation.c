#include "simulation.h"

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
    if (predator.p.distance_target == CLOSE)
        target.pv -= PREDATOR_DAMAGE;
}

void simulation_communicate(simulation_entity_t predator, simulation_entity_t * predators[NB_PREDATOR])
{
    int i;

    if (predator.p.cardinality_target != NOT_FOUND)
    {
        for (i = 0; i < NB_PREDATOR; ++i)
        {
            if (simulation_get_distance_between_2_predator(predator, *predators[i]) < COM_RADIUS)
            {
                predators[i]->p.cardinality_target = predator.p.cardinality_target;
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
    }
}

/* ------- PERCEPTIONS ---------- */

void simulation_get_closest_friend(simulation_entity_t * predators){
    float distmin=FLOAT_MAX,dist=0;
    int friend=0;
    for (int i=0;i<NB_PREDATOR;i++){
        for(int j=0;i<NB_PREDATOR;j++){
            if(i!=j){
                dist = (float) simulation_get_distance_between_2_predator(predators[i], predators[j]);
                if(dist<distmin){
                    dist=distmin;
                    friend=j;
                }
            }
        }
        predators[i].p.cardinality_friend=simulation_get_cardinals(predators[i].x,predators[friend].x,predators[i].y,predators[friend].y)
        predators[i].p.distance_friend=simulation_get_distance(distmin);
    }
}


enum distance simulation_get_distance(float distance){
    enum distance dist;
    if(distance>COM_RADIUS) dist=FAR;
    if(distance>COM_RADIUS) dist=CLOSE;
    return(dist);
}

enum direction_friend simulation_get_cardinals(float xa,float ya,float xb ,float yb)
{
    enum cardinality card;
    float
        deltax=xa - xb,
        deltay=ya - yb,
        delta=abs(deltax) - abs(deltay);

    if(deltax>0 && delta>0) card=WEST;
    if(deltax<0 && delta>0) card=EAST;
    if(deltay<0 && delta<0) card=SOUTH;
    if(deltay>0 && delta>0) card=NORTH;

    return(card);

}

int simulation_get_distance_between_2_predator(simulation_entity_t p2, simulation_entity_t p2)
{
    return abs(p2.x - predators[j].x) + abs(predators[i].y - predators[j].y)
}

void simulation_get_perception(simulation_entity_t * predators)
{
    enum distance dist;
    enum cardinality card;
    simulation_get_closest_friend(predators);
    for(int i=0;i<NB_PREDATOR;i++)
    {
        if(predators[i].p.direction_target!=NOT_FOUND){
            predators[i].p.direction_target=simulation_get_cardinals(predators[i].x,target.x,predators[i].y,target.y);
            predators[i].p.distance_target=simulation_get_distance((abs(predators[i].x - target.x) + abs(predators[i].y - target.y)));
        }
    }
}
