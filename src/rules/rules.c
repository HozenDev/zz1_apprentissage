#include "rules.h"



/**
 * @brief Allows you to save the rules structure in a file
 *
 * @param FILE * file       
 * @param char * path_file : if file == NULL, open file with path name and close
 * @param array_rules
 *
 */
void rules_save_file(FILE * file, rules_t array_rules[NB_RULES])
{
    for (int i=0 ; i < NB_RULES; ++i)
    {
        /* print perception */
        fprintf(file, "%d ", array_rules[i].perception.distance_friend);
        fprintf(file, "%d ", array_rules[i].perception.cardinality_friend);
        fprintf(file, "%d ", array_rules[i].perception.distance_target);
        fprintf(file, "%d ", array_rules[i].perception.cardinality_target);
	fprintf(file, "%d ", array_rules[i].perception.cardinality_density);
        /* print action and priority */
        fprintf(file, "%d %d \n", array_rules[i].action, array_rules[i].priority);
    }
}


/**
 * @brief Save an array of rules to a file specified by the given path.
 *
 * This function saves an array of rules to a file specified by the given path. If the path_file is NULL, the rules will be
 * saved to stdout instead. The rules are saved in a human-readable format.
 *
 * @param path_file     The path to the file where the rules should be saved. If NULL, the rules will be saved to stdout.
 * @param array_rules   The array of rules to be saved.
 */
void rules_save_path_file(char * path_file, rules_t array_rules[NB_RULES])
{
    FILE * file;
    if (path_file == NULL)   rules_save_file(stdout, array_rules);
    else
    {
	file = fopen(path_file, "w");
	
	if (NULL == file)
	{
	    zlog(stderr, ERROR, "Erreur open path_file, code error of errno %d", errno);
	    return;
	}
	
	rules_save_file(file, array_rules);
	fclose(file);
    }
}


/**
 * @brief Append an array of rules to a file specified by the given path, along with additional information.
 *
 * This function appends an array of rules to a file specified by the given path. If the path_file is NULL, the rules will be
 * appended to stdout instead. The rules are saved in a human-readable format. Additionally, the function includes information about
 * the iteration and the number of iterations in the file.
 *
 * @param path_file     The path to the file where the rules should be appended. If NULL, the rules will be appended to stdout.
 * @param array_rules   The array of rules to be appended.
 * @param iter          The current iteration.
 * @param k             The total number of iterations.
 */
void rules_save_path_file_a(char * path_file, rules_t array_rules[NB_RULES],int iter,int k)
{
    FILE * file;
    if (path_file == NULL)   rules_save_file(stdout, array_rules);
    else
    {
	file = fopen(path_file, "a");
	
	if (NULL == file)
	{
	    zlog(stderr, ERROR, "Erreur open path_file, code error of errno %d", errno);
	    return;
	}
	fprintf(file,"\niter:%d,en %d iter\n",k,iter);
	rules_save_file(file, array_rules);
	fclose(file);
    }
}

/**
 * @brief Allows you to retrieve the rules structure in a path_file
 *       
 * @param char * path_file : if file == NULL, open file with path name and close after
 * @param array_rules
 *
 */
void rules_read_path_file(char* path_file, rules_t array_rules[NB_RULES])
{ 
    FILE * file;
    file = fopen(path_file, "r");
    if (file == NULL)
    {
	zlog(stderr, ERROR, "Erreur open path_file, code error of errno %d", errno);
	return;
    }
    rules_read_file(file,array_rules);
    
    fclose(file);
}

/**
 * @brief Allows you to retrieve the rules structure in file
 *       
 * @param FILE * file 
 *
 */
void rules_read_file(FILE * file, rules_t array_rules[NB_RULES])
{
    if (NULL != file)
    {
	for (int i = 0; i < NB_RULES; ++i)
	{
            if (fscanf(file,"%d %d %d %d %d",
                       &array_rules[i].perception.distance_friend,
                       &array_rules[i].perception.cardinality_friend,
                       &array_rules[i].perception.distance_target,
                       &array_rules[i].perception.cardinality_target,
		       &array_rules[i].perception.cardinality_density) < 5)
            {
                zlog(stderr, ERROR, "Erreur lors de la lecture 1 des mesures à partir du fichier.\n", NULL); 
                return;
            }
	
	    if (fscanf(file, "%d %d\n", &array_rules[i].action, &array_rules[i].priority) != 2)
	    {
		zlog(stderr, ERROR, "Erreur lors de la lecture 2 des action et/ou priority à partir du fichier.\n", NULL);
		return;
	    }
	}
    }
    else
    {
	zlog(stderr, ERROR, "Impossible de lire dans le fichier car file est NULL.\n", NULL);
    }
}


/**
 * @brief Copy the contents of one array of rules to another.
 *
 * This function copies the contents of one array of rules to another. It iterates over each rule in the source array and
 * copies the values of the measures, action, and priority fields to the corresponding rule in the destination array.
 *
 * @param brainsrc   The source array of rules to be copied.
 * @param braindest  The destination array of rules where the contents will be copied.
 */
void rules_copy_brain(rules_t brainsrc[NB_RULES], rules_t braindest[NB_RULES])
{
    //for each rules in brain
    for (int i =0; i<NB_RULES; i++)
    {
        //copy measure
        braindest[i].perception.distance_friend     = brainsrc[i].perception.distance_friend;
        braindest[i].perception.cardinality_friend  = brainsrc[i].perception.cardinality_friend;
        braindest[i].perception.distance_target     = brainsrc[i].perception.distance_target;
        braindest[i].perception.cardinality_target  = brainsrc[i].perception.cardinality_target;
	braindest[i].perception.cardinality_density = brainsrc[i].perception.cardinality_density;

        //copy action
        braindest[i].action = brainsrc[i].action;

        //copy priority
        braindest[i].priority = brainsrc[i].priority;
    }
}


/**
 * @brief Copies the contents of one rules_t structure to another.
 *
 * This function copies the contents of the source `rules_t` structure to the destination
 * `rules_t` structure. It performs a deep copy, copying the measure values, action,
 * and priority from the source structure to the destination structure.
 *
 * @param brainsrc Pointer to the source `rules_t` structure.
 * @param braindest Pointer to the destination `rules_t` structure.
 *
 * @note The destination structure must be preallocated and have enough memory to store
 * the copied contents. No memory allocation is performed by this function.
 */
void rules_copy_rules(rules_t rules_src, rules_t * rules_dest)
{
    //copy measure
    (*rules_dest).perception.distance_friend     = rules_src.perception.distance_friend;
    (*rules_dest).perception.cardinality_friend  = rules_src.perception.cardinality_friend;
    (*rules_dest).perception.distance_target     = rules_src.perception.distance_target;
    (*rules_dest).perception.cardinality_target  = rules_src.perception.cardinality_target;
    (*rules_dest).perception.cardinality_density = rules_src.perception.cardinality_density;

    //copy action
    rules_dest->action = rules_src.action;

    //copy priority
    rules_dest->priority = rules_src.priority;
}

/**
 * @brief Copies the contents of one rules_t array to another.
 *
 * This function copies the contents of the source `rules_t` array to the destination
 * `rules_t` array. It performs a deep copy by calling the `rules_copy_rules` function
 * for each element of the arrays.
 *
 * @param brainsrc Pointer to the source `rules_t` array.
 * @param braindest Pointer to the destination `rules_t` array.
 *
 * @note The source and destination arrays must have enough memory to store the copied
 * contents. No memory allocation is performed by this function.
 */
void rules_copy_brain_genetic(rules_t brain_src[NB_RULES], rules_t brain_dest[NB_RULES])
{
    int i;
    for (i = 0; i<NB_RULES; ++i)
    {
	rules_copy_rules(brain_src[i], &brain_dest[i]);
    }
}
