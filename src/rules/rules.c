#include "rules.h"



/**
 * @brief Allows you to save the rules structure in a file
 *
 * @param FILE * file       
 * @param char * path_file : if file == NULL, open file with path name and close
 * @param array_rules
 *
 */
void rules_save_file(FILE * file, rules_t * array_rules)
{
    for (int i=0 ; i < NB_RULES; ++i)
    {
	for (int j=0; j<NB_MEASURE; ++j)
	{
	    fprintf(file, "%c ", array_rules[i].measures[j] + '0');
	}
	fprintf(file, "%c %c \n", array_rules[i].action + '0', array_rules[i].priority + '0');
    }
}


void rules_save_path_file(char * path_file, rules_t * array_rules)
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
 * @brief Allows you to retrieve the rules structure in a path_file
 *       
 * @param char * path_file : if file == NULL, open file with path name and close after
 * @param array_rules
 *
 */
void rules_read_path_file(char* path_file, rules_t * array_rules)
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
void rules_read_file(FILE * file, rules_t* array_rules)
{
    if (NULL != file)
    {
	for (int i = 0; i < NB_RULES; ++i)
	{
	    for (int j = 0; j < NB_MEASURE; ++j)
	    {
		if (fscanf(file,"%c ", &array_rules[i].measures[j]) != 1)
		{
		    zlog(stderr, ERROR, "Erreur lors de la lecture 1 des mesures à partir du fichier.\n", NULL); 
		    return;
		}
		array_rules[i].measures[j] -= '0'; // Conversion du caractère numérique à sa valeur entière
	    }
	
	    if (fscanf(file, "%c %c\n", &array_rules[i].action, &array_rules[i].priority) != 2)
	    {
		zlog(stderr, ERROR, "Erreur lors de la lecture 2 des action et/ou priority à partir du fichier.\n", NULL);
		return;
	    }
	    array_rules[i].priority -= '0'; // Conversion du caractère numérique à sa valeur entière
	    array_rules[i].action -= '0'; // Conversion du caractère numérique à sa valeur entière
	}
    }
    else
    {
	zlog(stderr, ERROR, "Impossible de lire dans le fichier car file est NULL.\n", NULL);
    }
}



/**
 * @brief Allows you to create array_rules in memory
 *
 * @param rules_t ** array_rules : adress of array_rules want create
 *
 */
void rules_create_array_rules(rules_t ** array_rules)
{
    (*array_rules) = (rules_t *) malloc(sizeof(rules_t)*NB_RULES);
}

/**
 * @brief Allows you to destroy array_rules in memory
 *
 * @param rules_t ** array_rules : adress of array_rules want create
 *
 */
void rules_destroy_array_rules(rules_t * array_rules)
{
    free(array_rules);
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
void rules_copy_rules(rules_t rules_src, rules_t rules_dest)
{
    //copy measure
    for (int j=0; j<NB_MEASURE; j++)
	rules_dest.measures[j] = rules_src.measures[j];

    //copy action
    rules_dest.action = rules_src.action;

    //copy priority
    rules_dest.priority = rules_src.priority;
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
void rules_copy_brain(rules_t brain_src[NB_RULES], rules_t brain_dest[NB_RULES])
{
    int i;
    for (i = 0; i<NB_RULES; ++i)
    {
	rules_copy_rules(brain_src[i], brain_dest[i]);
    }
}
