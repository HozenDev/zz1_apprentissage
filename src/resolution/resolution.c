#include "../seed/seed.h"
#include "../rules/rules.h"

void resolution_recuis_simule(float (*pf)(float),rules_t ** brain,float * temps_min,int * score_max)
{
    /* paramètres */
    float temps=0,temperature = 1000,espsilon = 0.001;
    int score=0;
    temps_min=50000;
    score_max=0;
    generate_seed(0);


    /* allocation des cerveaux initial et suivant*/
    *brain = (rules_t *) malloc(sizeof(rules_t)*NB_RULES);
    rules_t *new = (rules_t *) malloc(sizeof(rules_t)*NB_RULES);


    //lit la solution initiale
        
    
    //to do test init 
    !

    while (temperature > espsilon)
    {
        rules_copy_brain(brain,new);
        
        resolution_random_change(&new);

        //to do test new
        
        if(score_max<score)// compare score 
        {
            *solution=new;
            score_max=score;
        }
        if(temps_min>temps ) // to do compare time and test if score is maximal
        {
            *solution=new;
            temps_min=temps;
        }
        temperature=pf(temperature);
    }

     //to do liberer new
    
}   


void resolution_random_change(rules_t ** brain)
{
    //selecting modified rules
    int random_rule=rand()%10;
    //selecting modified attributes
    int random_attribute=rand()%5;
    int possibility[5]={4,4,3,3,3}
    
    //apllying modification
    switch (random_attribute)
    {
    case 1:
        brain[random_rule]->measures[random_attribute]=rand()%possibility[random_attribute];
    case 2:
        brain[random_rule]->measures[random_attribute]=rand()%possibility[random_attribute];
    case 3: 
        brain[random_rule]->measures[random_attribute]=rand()%possibility[random_attribute];
    case 4:
        brain[random_rule]->action=rand()%possibility[random_attribute];
    case 5:
        brain[random_rule]->priority=rand()%possibility[random_attribute];
        
    }
}
/*
void resolution_gloutone_locale(rules_t ** brain,float * temps_min,int * score_max)
{
    //paramètres     
    float temps=0,temperature = 1000,espsilon = 0.001;
    int score=0;
    temps_min=50000;
    score_max=0;
    generate_seed(0);


    // allocation des cerveaux initial et suivant
    *brain = (rules_t *) malloc(sizeof(rules_t)*NB_RULES);
    rules_t *new = (rules_t *) malloc(sizeof(rules_t)*NB_RULES);

    //lit la solution initiale
}
*/