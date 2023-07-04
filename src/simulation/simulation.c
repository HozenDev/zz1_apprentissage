#include "simulation.h"

int target_x = WORLD_WIDTH/2;
int target_y = WORLD_HEIGHT/2;

void simulation_create(void)
{
    /* todo */
}

void simulation_free(void)
{
    /* todo */
}

int simulation_communicate()
{
    
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

void game_get_closest_friend(simulation_entity_t * predators){
    
    float distmin=FLOAT_MAX,dist=0;
    int friend=0;
    for (int i=0;i<NB_PREDATOR;i++){
        for(int j=0;i<NB_PREDATOR;j++){
            if(i!=j){
                dist=sqrt((predators.x[i] - predators[j]) ** 2 + (predators[i].y - predators[j].y) ** 2)
                if(dist<distmin){
                    dist=distmin;
                    friend=j;
                }
            }
        }

        
    }
}

enum direction_friend game_get_cardinals(float xa,float ya,float xb ,float yb){
    enum direction_friend card;
    float deltax=xa - xb,
          deltay=ya - yb,
          delta=abs(deltax) - abs(deltay)
    if(deltax>0 && delta>0) card=west;

    if(deltax<0 && delta>0) card=east;

    if(deltay<0 && delta<0) card=south;

    if(deltay>0 && delta>0) card=north;

    return(card);

}
