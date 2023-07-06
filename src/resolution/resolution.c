#include "../resolution/resolution.h"

void init_random_brain(rules_t * brain){
    for(int i=0;i<NB_RULES;i++){
        brain[i].perception.distance_friend=(rand()%NB_DISTANCE) - 1;
        brain[i].perception.cardinality_friend=rand()%(NB_CARDINALITY-1) - 1;
        brain[i].perception.distance_target=rand()%NB_DISTANCE - 1;
        brain[i].perception.cardinality_target=rand()%NB_CARDINALITY - 1;
        brain[i].action=rand()%NB_ACTION ;
        brain[i].priority=rand()%NB_PRIORITY ;

    }
}

void resolution_recuis_simule(float (*pf)(float), char * path_brain_load, char * path_brain_res, int * res)
{
    /* paramètres */
    float temperature = ITER_MAX;
    float epsilon = EPSILON;
    int score_min = INT_MAX; 
    int score;
    
    /* creation des cerveaux */
    rules_t brain[NB_RULES];
    rules_t new_brain[NB_RULES];

    rules_read_path_file(path_brain_load, brain);

    /* genere l'aleatoire*/
    // generate_seed(0);

    simulation_loop_average(brain, &score_min);
    
    /* recuit simulé */
    while (temperature > epsilon)
    {
	/* generation voisin*/
        rules_copy_brain(brain, new_brain);
        resolution_random_change(new_brain);

        /* fait jouer new*/
	simulation_loop_average(new_brain, &score);
        /* comparaison ou proba*/
	if (score_min > score ||
	    ( rand() / (float)RAND_MAX < exp(-fabs((score - score_min) / temperature)) &&  score_min*1.3 > score) )
	    // to do compare time and test if score is maximal
        {
	    zlog(stdout, INFO, "changement de cerveau, score %d\n", score);
            rules_copy_brain(new_brain, brain);
            score_min=score;
	    rules_save_file(stdout, brain);
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




void resolution_gloutone_aleatoire(rules_t brain[NB_RULES],int* iterret)
{
    //paramètres    
    int itermin=ITER_MAX;
    int indice_rule=0;
    int parcours[NB_RULES*(NB_MEASURE + 2)];
    int best_j=0,j=0;
    int iter=0;

    // allocation des cerveaux initial et suivant
    rules_t new[NB_RULES];
    printf("laezkaeml\n");

    /* init le parcours */
    for(int i=0;i<NB_RULES*(NB_MEASURE + 2);i++) parcours[i]=i;

    /* melange le parcours*/
    utils_shuffle(parcours,NB_RULES*(NB_MEASURE + 2));

    //init_random_brain(brain);
    rules_save_file(stdout,brain);
    rules_copy_brain(brain, new);
    for (int k=0;k<10;k++)
    {
        utils_shuffle(parcours,NB_RULES*(NB_MEASURE + 2));

        printf("laezkaeml\n");
    
        for(int i=0;i<NB_RULES*(NB_MEASURE + 2);i++)
        {
            rules_copy_brain(brain, new);

            printf("laezkaeml\n");

            best_j=-2;
            indice_rule=parcours[i]/6;
            switch(parcours[i]%6)
            {
                case 0:
                    for(j=0;j<NB_DISTANCE;j++){
                        iter=0;
                        new[indice_rule].perception.distance_friend=j-1;
                        simulation_loop_average(new,&iter);
                        if(iter<itermin)
                        {
                            itermin=iter;
                            best_j=j-1;
                        }
                        if(best_j!=-2) brain[indice_rule].perception.distance_friend=best_j;

                    }
                    break;
                case 1:
                    for(j=0;j<NB_CARDINALITY-1;j++)
                    {
                        iter=0;
                        new[indice_rule].perception.cardinality_friend=j-1;
                        simulation_loop_average(new,&iter);
                        if(iter<itermin)
                        {
                            itermin=iter;
                            best_j=j-1;
                        }
                        if(best_j!=-2)brain[indice_rule].perception.cardinality_friend=best_j;

                    }
                    break;
                case 2:
                    for(j=0;j<NB_DISTANCE;j++){
                        iter=0;
                        new[indice_rule].perception.distance_target=j-1;
                        simulation_loop_average(new,&iter);
                        if(iter<itermin)
                        {
                            itermin=iter;
                            best_j=j-1;

                        }
                        if(best_j!=-2) brain[indice_rule].perception.distance_target=best_j;
                    }
                    break;

                case 3:
                    for(j=0;j<NB_CARDINALITY;j++)
                    {
                        iter=0;
                        new[indice_rule].perception.cardinality_target=j-1;
                        simulation_loop_average(new,&iter);
                        if(iter<itermin)
                        {
                            itermin=iter;
                            best_j=j-1;
                        }
                        if(best_j!=-2) brain[indice_rule].perception.cardinality_target=best_j;

                    }
                    break;
                case 4:
                    for(j=0;j<NB_ACTION;j++){
                        iter=0;
                        new[indice_rule].action=j;
                        simulation_loop_average(new,&iter);
                        if(iter<itermin)
                        {
                            itermin=iter;
                            best_j=j;
                        }
                        if(best_j!=-2) brain[indice_rule].action=best_j;
                    }
                    break;
                case 5:
                    for(j=0;j<NB_PRIORITY;j++){
                        iter=0;
                        new[indice_rule].priority=j;
                        simulation_loop_average(new,&iter);
                        if(iter<itermin)
                        {
                            itermin=iter;
                            best_j=j;
                        }
                        if(best_j!=-2) brain[indice_rule].priority=best_j;
                    }
                    break;
            }
            printf("laezkaem23l\n");
            
            zlog(stdout ,INFO,"%d iter,%d i",iter,i)
            *iterret=itermin;
        }
    //rules_save_path_file_a("../data/rules_glouton.txt", brain,*iterret,k);
    }
}