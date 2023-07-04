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
void simulation_get_perception(simulation_entity_t * predators){
    enum distance dist;
    enum cardinality card;
    simulation_get_closest_friend(predators,rules);
    for(int i=0;i<NB_PREDATOR,i++)
    {
        if(predators[i].p.direction_target!=NOT_FOUND){
            predators[i].p.direction_target=simulation_get_cardinals(predators[i].x,target.x,predators[i].y,target.y)
            predators[i].p.distance_target=simulation_get_distance((predators[i].x - target.x + predators[i].y - target.y))
        }
    }
}

int simulation_communicate(void)
{
    /* todo */
}

void simulation_move_entity(simulation_entity_t predators, enum cardinality c)
{
    switch (c)
    {
    case NORTH:
        predators.y -= PREDATOR_SPEED;
        if (predators.y < 0) predators.y = 0;
        break;
    case SOUTH:
        predators.y += PREDATOR_SPEED;
        if (predators.y > WORLD_HEIGHT) predators.y = WORLD_HEIGHT;
        break;
    case EAST:
        predators.x += PREDATOR_SPEED;
        if (predators.x > WORLD_WIDTH) predators.y = WORLD_WIDTH;
        break;
    case WEST:
        predators.x -= PREDATOR_SPEED;
        if (predators.y < 0) predators.y = 0;
        break;
    }
}

void simulation_get_closest_friend(simulation_entity_t * predators,rules_t ** rules){
    float distmin=FLOAT_MAX,dist=0;
    int friend=0;
    for (int i=0;i<NB_PREDATOR;i++){
        for(int j=0;i<NB_PREDATOR;j++){
            if(i!=j){
                dist=(predators[i].x - predators[j].x)  + (predators[i].y - predators[j].y)
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
    if(distance>com_radius) dist=far;

    if(distance>com_radius) dist=close;

    return(dist);

}
enum direction_friend simulation_get_cardinals(float xa,float ya,float xb ,float yb){
    enum cardinality card;
    float deltax=xa - xb,
          deltay=ya - yb,
          delta=abs(deltax) - abs(deltay)
    if(deltax>0 && delta>0) card=west;

    if(deltax<0 && delta>0) card=east;

    if(deltay<0 && delta<0) card=south;

    if(deltay>0 && delta>0) card=north;

    return(card);

}
