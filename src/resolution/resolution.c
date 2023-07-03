#include "../seed/seed.h"

float resolution_recuis_simule(float (*pf)(float),regle_t ** solution)
{
    /* paramètres */
    float temps_min=50000;
    float distance=0;
    float temperature = 1000, //la distance maximale possible entre 2 points i.e. la diagonale de la fenetre
          espsilon = 0.001;
    int indicerand=0;
    int indicerand2=0;
    generate_seed(0);


    /* allocation des tableaux de solutions et solutions + 1 */
    int * new = (int *) malloc(sizeof(int)*taille);

    *solution = (int *) malloc(sizeof(int)*taille);


    //to do lire la solution initiale dans un fichier

    
    
    //to do test init 
    

    while (temperature > espsilon)
    {
        // to do copy brain
        
        
        //to do swap two values in the new brain

        //to do test new
        
        //to do check if new and solution killed every jellyfishes 
        
        if( rand()/RAND_MAX<exp(-(distance-distmin)/temperature)) // to do compare results
        {
            // to do update solution with new values

        }
        temperature=pf(temperature);
    }

    
    free(new);
    return distmin;
}   


void resolution_random_change(regle_t ** solution)
{
    //selecting modified rules
    int random_rule=rand()%10;
    //selecting modified attributes
    int random_attribute=rand()%5;
    
    
    //apllying modification
    switch (random_attribute)
    {
    case 1:
        // to do replace 1st value within measures
    case 2:
        // to do replace 2nd value within measures
    case 3: 
        //to  do replace 3rd value within measures
    case 4:
        //to do replace value of action
    case 5:
        //to do replace value of priority
    }
}