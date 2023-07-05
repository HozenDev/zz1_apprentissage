#include "../resolution/resolution.h"

void resolution_recuis_simule(float (*pf)(float), char * path_brain_load, char * path_brain_res, int * res)
{
    /* paramètres */
    float temperature = TEMP_DEP, espsilon = EPSILON;
    int score_min = INT_MAX; 
    int score;
    
    /* genere l'aleatoire*/
    generate_seed(0);

    /* creation des cerveaux */
    rules_t brain[NB_RULES];
    rules_t new_brain[NB_RULES];

    rules_read_path_file(path_brain_load, brain);
    
    /* recuit simulé */
    while (temperature > espsilon)
    {
        /* generation voisin*/
        rules_copy_brain(brain, new_brain);
	
        resolution_random_change(new_brain);

        /* fait jouer new*/
	simulation_loop(new_brain, &score);
      
        /* comparaison ou proba*/
        if(score_min > score || rand()/RAND_MAX< exp(-abs((score-score_min))/temperature))
           // to do compare time and test if score is maximal
        {
            rules_copy_brain(new_brain, brain);
            score_min=score;
        }

        /*modification temperature*/
        temperature=pf(temperature);
    }
    *res = score_min;
    rules_save_path_file(path_brain_res, brain);
}   


void resolution_random_change(rules_t brain[NB_RULES])
{
    //selecting modified rules
    int random_rules   = rand()%NB_RULES;
    int random_indice = rand()%NB_MEASURE+2;

    if(random_indice == 0)
	brain[random_rules].perception.distance_friend = (rand()%NB_DISTANCE)-1;       
    else if (random_indice == 1)
	brain[random_rules].perception.distance_target = (rand()%NB_DISTANCE)-1;
    else if (random_indice == 2)
	brain[random_rules].perception.cardinality_friend = (rand()%(NB_CARDINALITY-1))-1;
    else if (random_indice == 3)
	brain[random_rules].perception.cardinality_target = (rand()%NB_CARDINALITY)-1;
    else if (random_indice == 4)
	brain[random_rules].action = (rand()%NB_ACTION)-1;
    else if (random_indice == 5)
	brain[random_rules].priority = (rand()%NB_PRIORITY)-1;
    else
	fprintf(stderr, "Erreur dans resolution_random_change, random_indice supérieur au nombre de champs de rules");
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
    

    //lit la solution initiale
}
*/
