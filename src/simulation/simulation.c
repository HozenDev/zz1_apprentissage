#include "simulation.h"
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


void simulation_get_closest_friend(simulation_entity_t * predators,rules_t ** rules){
    
    float distmin=FLOAT_MAX,dist=0;
    int friend=0;
    for (int i=0;i<NB_PREDATOR;i++){
        for(int j=0;i<NB_PREDATOR;j++){
            if(i!=j){
                dist=(predators.x[i] - predators[j])  + (predators[i].y - predators[j].y)
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