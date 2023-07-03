float resolution_recuis_simule(float (*pf)(float),regle_t ** solution)
{
    /* paramètres */
    float temps_min=50000;
    float distance=0;
    float temperature = 1000, //la distance maximale possible entre 2 points i.e. la diagonale de la fenetre
          espsilon = 0.001;
    int indicerand=0;
    int indicerand2=0;


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
