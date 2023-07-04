#include "../seed/seed.h"
#include "../rules/rules.h"

void resolution_recuis_simule(float (*pf)(float),rules_t ** brain,float * temps_min)
{
    /* paramètres */
    float temps=0,temperature = 1000,espsilon = 0.001;
    temps_min=50000;
    /* genere l'aleatoire*/
    generate_seed(0);


    /* allocation des cerveaux initial et suivant*/
    *brain = (rules_t *) malloc(sizeof(rules_t)*NB_RULES);
    rules_t *new = (rules_t *) malloc(sizeof(rules_t)*NB_RULES);

    /* init solution */
    //lit la solution initiale
        
    /* fait jouer solution */
    //to do test init 
    
    /* recuit simulé */
    while (temperature > espsilon)
    {
        /* generation voisin*/
        rules_copy_brain(brain,new);
        resolution_random_change(&new);

        /* fait jouer new*/
        //to do test new
      
        /* comparaison ou proba*/
        if(temps_min>temps || rand()/RAND_MAX<exp(-abs((temps-temps_min))/temperature)) // to do compare time and test if score is maximal
        {
            rules_copy_brain(new,brain);
            temps_min=temps;
        }

        /*modification temperature*/
        temperature=pf(temperature);
    }
    /* liberation new */
     //to do liberer new
    
}   

/*
void resolution_random_change(rules_t ** brain)
{
    //selecting modified rules
    int random_rule=rand()%10;
    //selecting modified attributes
    int random_attribute=rand()%6;
    
    //apllying modification
    switch (random_attribute)
    {
    case 0:
        brain[random_rule].perception.distance_friend=rand()%3;
    case 1:
        brain[random_rule].perception.distance_friend=rand()%3;
    case 2: 
        brain[random_rule].perception.distance_friend=rand()%3;
    case 3:
        brain[random_rule].perception.distance_=rand()%5;
    case 4:
        brain[random_rule].priority=rand()%3;
    case 5:
        brain[random_rule].action=rand()%6;
    }
}
*/
void init_random_brain(rules_t * brain){
    for(int i=0;i<NB_RULES;i++){
        brain[i].perception.distance_friend=rand()%NB_DISTANCE - 1;
        brain[i].perception.cardinality_friend=rand()%NB_MEASURES - 1;
        brain[i].perception.distance_target=rand()%;
        brain[i].perception.cardinality_target=rand()%6;
    }
}
void resolution_gloutone_locale(rules_t ** brain)
{
    //paramètres     
    int indice_rule=0;
    int parcours[NB_RULES*(NB_MEASURES + 2)];
    /* init le parcours */
    for(int i=0;i<NB_RULES*(NB_MEASURES + 2);i++) parcours[i]=i;

    /* melange le parcours*/
    utils_shuffle(parcours,NB_RULES*(NB_MEASURES + 2));


    // allocation des cerveaux initial et suivant
    rules_t new[10];

    init_random_brain(*brain);

    rules_copy_brain(brain,new);

    for(int i=0;i<NB_RULES*(NB_MEASURES + 2);i++)
    {
        indice_rule=i/6;
        switch(i%6)
        {
            case 0:
            for(j=0;j)
                (*brain)[i].perception.distance_friend=
        }
    }

}
