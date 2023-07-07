#include "../resolution/resolution.h"


/**
 * @brief Initialize the brain with random values.
 *
 * This function initializes the brain with random values. It generates random values for each rule in the brain's perception
 * and action fields. The random values are generated within the specified ranges: distance_friend [0, NB_DISTANCE-1],
 * cardinality_friend [0, NB_CARDINALITY-2], distance_target [0, NB_DISTANCE-1], cardinality_target [0, NB_CARDINALITY-1],
 * action [0, NB_ACTION-1], priority [1, NB_PRIORITY]. The generated random values are assigned to the corresponding fields
 * in each rule of the brain.
 *
 * @param brain  Pointer to the brain to be initialized.
 */
void init_random_brain(rules_t * brain){
    for(int i=0;i<NB_RULES;i++){
        brain[i].perception.distance_friend     = (rand()%NB_DISTANCE) - 1;
        brain[i].perception.cardinality_friend  = rand()%(NB_CARDINALITY-1) - 1;
        brain[i].perception.distance_target     = rand()%NB_DISTANCE - 1;
        brain[i].perception.cardinality_target  = rand()%NB_CARDINALITY - 1;
	brain[i].perception.cardinality_density = rand()%NB_CARDINALITY -1 ;
        brain[i].action                         = rand()%NB_ACTION ;
        brain[i].priority                       = rand()%NB_PRIORITY + 1 ;

    }
}

/**
 * @brief Initializes a random brain by assigning random values to each rule.
 *
 * This function initializes a brain structure by assigning random values to each
 * rule's perception and action properties. The random values are generated based on
 * predefined constants (NB_RULES, NB_DISTANCE, NB_CARDINALITY, NB_ACTION, NB_PRIORITY).
 *
 * @param brain A pointer to the brain structure to be initialized.
 */
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
	zlog(stdout, DEBUG, "Temperature %f epsilon %f\n", temperature, epsilon);
	/* generation voisin*/
        rules_copy_brain(brain, new_brain);
        resolution_random_change(new_brain);

        /* fait jouer new*/
	simulation_loop_average(new_brain, &score);
        /* comparaison ou proba*/
	if (score_min > score ||
	    (float) rand() / (float) RAND_MAX < exp(-fabs((score - score_min) / temperature)))
	    // to do compare time and test if score is maximal
        {
	    zlog(stdout, DEBUG, "Changement de cerveau, score %d\n", score);
            rules_copy_brain(new_brain, brain);
            score_min=score;
        }
        /*modification temperature*/
        temperature=pf(temperature);
    }
    *res = score_min;
    rules_save_path_file(path_brain_res, brain);
}   



/**
 * @brief Modifies a random rule in the brain by changing one of its properties randomly.
 *
 * This function selects a random rule from the brain and modifies one of its properties
 * randomly. The properties that can be modified include perception distance of friend,
 * perception distance of target, perception cardinality of friend, perception cardinality
 * of target, action, and priority. The random property to be modified is determined by
 * generating a random index (random_indice) based on the predefined constant NB_MEASURE.
 *
 * @param brain An array of rules representing the brain.
 */
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
	brain[random_rules].action = (rand()%NB_ACTION);
    else if (random_indice == 5)
	brain[random_rules].priority = (rand()%NB_PRIORITY + 1);
    else if (random_indice == 6)
	brain[random_rules].perception.cardinality_density = (rand()%(NB_CARDINALITY-1))-1;
    else
	fprintf(stderr, "Erreur dans resolution_random_change, random_indice supérieur au nombre de champs de rules");
}



/**
 * @brief Resolves the brain using a greedy random approach by iteratively modifying rules.
 *
 * This function resolves the brain by iteratively modifying rules using a greedy random approach.
 * It randomly selects a rule and one of its properties to modify. The modification is based on
 * evaluating the performance of the brain with the modified rule using the `simulation_loop_average`
 * function. The function repeats this process for a specified number of iterations.
 *
 * @param brain    An array of rules representing the brain.
 * @param iterret  A pointer to an integer to store the minimum iteration value obtained during resolution.
 */
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
    
        for(int i=0;i<NB_RULES*(NB_MEASURE + 2);i++)
        {
            rules_copy_brain(brain, new);
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
            
            zlog(stdout, DEBUG, "%d iter, %d i", iter, i);
            *iterret=itermin;
        }
    //rules_save_path_file_a("../data/rules_glouton.txt", brain,*iterret,k);
    }
}
