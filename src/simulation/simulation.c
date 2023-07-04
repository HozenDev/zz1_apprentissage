#include "simulation.h"
#include "rules.h"
#include "const.h"

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
void simulation_get_perception(simulation_entity_t * predators)
{
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

void simulation_destroy_target(simulation_entity_t predators)
{
    if (predators.perception.distance_target == CLOSE) {
        target.pv -= PREDATOR_DAMAGE;
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
        if (simulation_verify_rules(predators,brain[i])){
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

void simulation_execute_action(simulation_entity_t predator,int action,simulation_entity_t * predators){
    switch(action)
    {
    case 0:
        simulation_move_entity(predator,NORTH);
    case 1:
        simulation_move_entity(predator,SOUTH);
    case 2:
        simulation_move_entity(predator,EAST);
    case 3:
        simulation_move_entity(predator,WEST);
    case 4:
        simulation_communicate(predator,predators);
    case 5:
        simulation_destroy_target(predator);
    }
}

void simulation_loop(){
    //simulation_init();
    int iter=0;
    while(target.pv!=0 && iter<iter_max)
    {
        
    }
}