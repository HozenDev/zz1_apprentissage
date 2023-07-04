#include "simulation.h"
#include "rules.h"
void simulation_create(void)
{
    /* todo */
}

void simulation_free(void)
{
    /* todo */
}

void game_get_closest_friend(simulation_entity_t * predators,rules_t ** rules){
    
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
        (*rules)[i]->measure[1]=game_get_cardinals(predators[i].x,predators[friend].x,predators[i].y,predators[friend].y)
        (*rules)[i]->measure[0]
    }
}
enum distance game_get_distance(float distance){
    
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