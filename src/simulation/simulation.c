#include "simulation.h"

int target_x = WORLD_WIDTH/2;
int target_y = WORLD_HEIGHT/2;

#include "rules.h"
#include "const.h"

void simulation_create(void)
{
    /* todo */
}

void simulation_free(void)
{
    /* todo */
}
void simulation_get_perception(simulation_entity_t * predators ){
    enum distance dist;
    enum cardinality card;
    simulation_get_closest_friend(predators,rules);
    for(int i=0;i<NB_PREDATOR,i++)
    {
        if((*rules)[i]->measure[2]!=0){
            (*rules)[i]->measure[2]=simulation_get_cardinals(predators[i].x,target.x,predators[i].y,target.y)
            (*rules)[i]->measure[3]=simulation_get_distance((predators[i].x - target.x + predators[i].y - target.y))
        }
    }
}

int simulation_get_distance_between_2_predator(simulation_entity_t p2, simulation_entity_t p2)
{
    return abs(p2.x - predators[j].x) + abs(predators[i].y - predators[j].y)
}

int simulation_communicate(simulation_entity_t predator, simulation_entity_t * predators[NB_PREDATOR])
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
    }
}

void simulation_get_closest_friend(simulation_entity_t * predators,rules_t ** rules){
    float distmin=FLOAT_MAX,dist=0;
    int friend=0;
    for (int i=0;i<NB_PREDATOR;i++){
        for(int j=0;i<NB_PREDATOR;j++){
            if(i!=j){
                dist = simulation_get_distance_between_2_predator(predators[i], predators[j]);
                if(dist<distmin){
                    dist=distmin;
                    friend=j;
                }
            }
        }
        (*rules)[i]->measure[1]=simulation_get_cardinals(predators[i].x,predators[friend].x,predators[i].y,predators[friend].y)
        (*rules)[i]->measure[0]=simulation_get_distance(distmin);
    }
}


enum distance simulation_get_distance(float distance){
    enum distance dist;
    if(distance>COM_RADIUS) dist=FAR;

    if(distance>com_radius) dist=CLOSE;

    return(dist);

}
enum direction_friend simulation_get_cardinals(float xa,float ya,float xb ,float yb){
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
