#include "simulation.h"
#include <stdlib.h>
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
    simulation_get_closest_friend(predators);
    for(int i=0;i<NB_PREDATOR,i++)
    {
        if(predators[i].p.direction_target!=NOT_FOUND){
            predators[i].p.direction_target=simulation_get_cardinals(predators[i].x,target.x,predators[i].y,target.y)
            predators[i].p.distance_target=simulation_get_distance((abs(predators[i].x - target.x) + abs(predators[i].y - target.y)))
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

void simulation_get_closest_friend(simulation_entity_t * predators){
    float distmin=FLOAT_MAX,dist=0;
    int friend=0;
    for (int i=0;i<NB_PREDATOR;i++){
        for(int j=0;i<NB_PREDATOR;j++){
            if(i!=j){
                dist=abs(predators[i].x - predators[j].x)  + abs(predators[i].y - predators[j].y)
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

int simulation_verify_rules(simulation_entity_t predators,rules_t rule){
    int flag=1;
    /*test distance friend*/
    if(preadtors.p.distance_friend!=JOKER_D && predators.p.distance_friend==rule.perception.distance_friend) flag=0;
   
    /*test distance target*/
    if(preadtors.p.distance_target!=JOKER_D && predators.p.distance_target==rule.perception.distance_target) flag=0;
    
    /*test cardinality friend*/
    if(preadtors.p.cardinality_friend!=JOKER_C && predators.p.cardinality_friend==rule.perception.cardinality_friend) flag=0;
    
    /*test direction target*/
    if(preadtors.p.direction_friend!=JOKER_C && predators.p.direction_friend==rule.perception.direction_friend) flag=0;

    return(flag);


}

int * simulation_filtrage_regle(simulation_entity_t predators,rules_t * brain){
    int filtered_rules[NB_RULES]={0};
    int flag=0;
    for(int i=0;i<NB_RULES;i++){
        if (simulation_verify_rules(predatros,brain[i])){
            filtered_rules[i]=1;
        }
    }
}

int simulation_choose_action(int * filtered_rules,rules_t * brain)
{
    int sum=0;
    int probability[NB_RULES]={0};
    int cumulativeProbability=0;
    int action=0;
    for(j=0;j<NB_RULES;j++){
        if(filtered_rules[j]==1){
            sum+=brain[i].priority**s_power;
            probability[i]=brain[i].priority**s_power;
        }
    }

    for(j=0;j<NB_RULES;j++){
        if(filtered_rules[j]==1){
            cumulativeProbability+=probability[i]/sum;
            if(rand()/RAND_MAX<cumulativeProbability){
                action=i;
            }
            
        }
    }
    return(action);
}