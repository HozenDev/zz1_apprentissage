#include "simulation.h"
#include <math.h>
#include <limits.h>

#include <stdlib.h>
#include "../utils/utils.h"
void simulation_create(void)
{
    /* todo */
}

void simulation_free(void)
{
    /* todo */
}

/* ------- ACTIONS -------- */


/**
 * @brief Destroys the target in the simulation if the predator is close enough.
 *
 * This function destroys the target in the simulation if the predator is close enough
 * to it. If the distance between the predator and the target is defined as "CLOSE",
 * the target's health points are reduced by the value of `PREDATOR_DAMAGE`.
 *
 * @param predator The simulation entity representing the predator.
 *
 * @note The function assumes that the `target` object and the `PREDATOR_DAMAGE` constant
 * are defined and accessible within the scope of this function.
 */
void simulation_destroy_target(entity_t predator, target_t * target)
{
    if (predator.p.distance_target == CLOSE) {
        target->pv -= PREDATOR_DAMAGE;
    }
}

/**
 * @brief Communicates the target information to nearby predators in the simulation.
 *
 * This function communicates the target information from the given `predator` to other
 * nearby predators in the simulation. It checks if the `predator` has a valid target
 * (`cardinality_target != NOT_FOUND`). If so, it iterates through the `predators` array
 * and for each predator within the communication radius (`COM_RADIUS`), it updates their
 * `cardinality_target` with the value from the `predator`.
 *
 * @param predator  The simulation entity representing the predator that communicates.
 * @param predators An array of simulation entities representing all predators in the simulation.
 *
 * @note The function assumes that the `NB_PREDATOR`, `COM_RADIUS`, and
 * `simulation_get_distance_between_2_predator` function are defined and accessible within
 * the scope of this function.
 */
void simulation_communicate(entity_t predator, entity_t predators[NB_PREDATOR])
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

/**
 * @brief Moves the predator entity based on the given direction.
 *
 * This function moves the predator entity in the specified direction (`c`) by modifying
 * its coordinates (`x` and `y`) according to the `PREDATOR_SPEED` constant. The function
 * also checks for boundary conditions and ensures that the predator stays within the
 * defined world limits (`WORLD_WIDTH` and `WORLD_HEIGHT`).
 *
 * @param predator A pointer to the simulation entity representing the predator.
 * @param c        The direction in which the predator should move.
 *
 * @note The function assumes that the `PREDATOR_SPEED`, `WORLD_WIDTH`, and `WORLD_HEIGHT`
 * constants are defined and accessible within the scope of this function.
 */
void simulation_move_entity(entity_t * predator, enum cardinality c)
{
    switch (c)
    {
    case NORTH:
        predator->y -= PREDATOR_SPEED;
        if (predator->y < 0) predator->y = 0;
        break;
    case SOUTH:
        predator->y += PREDATOR_SPEED;
        if (predator->y > WORLD_HEIGHT) predator->y = WORLD_HEIGHT;
        break;
    case EAST:
        predator->x += PREDATOR_SPEED;
        if (predator->x > WORLD_WIDTH) predator->x = WORLD_WIDTH;
        break;
    case WEST:
        predator->x -= PREDATOR_SPEED;
        if (predator->x < 0) predator->x = 0;
        break;
    case NOT_FOUND:
        /* to do */
        break;
    case JOKER_C:
        /* todo */
        break;
    default :
	fprintf(stderr, "Erreur c in simulation_move_entity, value of c not possible\n");
	break;
    }
}


/* ------- PERCEPTIONS ---------- */
/**
 * @brief Determines the cardinality based on the coordinates of two points.
 *
 * This function determines the cardinality (direction) based on the coordinates of two
 * points (`xa`, `ya` and `xb`, `yb`). It calculates the differences in x and y coordinates
 * (`deltax` and `deltay`) and the delta value (`delta`) based on the absolute differences
 * between `deltax` and `deltay`. Then, it assigns the appropriate cardinality value to the
 * `card` variable based on the calculated differences and returns it.
 *
 * @param xa The x-coordinate of the first point.
 * @param ya The y-coordinate of the first point.
 * @param xb The x-coordinate of the second point.
 * @param yb The y-coordinate of the second point.
 *
 * @return The cardinality (direction) based on the coordinates of the two points.
 *
 * @note The function assumes that the `cardinality` enum and the `abs` function are
 * defined and accessible within the scope of this function.
 */
enum cardinality simulation_get_cardinals(float xa,float ya,float xb ,float yb)
{
    enum cardinality card = NORTH;
    int 
        deltax=xa - xb,
        deltay=ya - yb,
        delta=abs(deltax) - abs(deltay);

    if     (deltax>0 && delta>0) card=WEST;
    else if(deltax<0 && delta>0) card=EAST;
    else if(deltay<0 && delta<0) card=SOUTH;

    return(card);
}

/**
 * @brief Determines the closest friend for each predator in the simulation.
 *
 * This function determines the closest friend for each predator in the simulation by
 * calculating the distance between each pair of predators and finding the minimum
 * distance. For each predator, it updates the `cardinality_friend` with the cardinality
 * (direction) of the closest friend, and `distance_friend` with the minimum distance to
 * the closest friend.
 *
 * @param predators An array of simulation entities representing all predators in the simulation.
 *
 * @note The function assumes that the `NB_PREDATOR`,
 * `simulation_get_distance_between_2_predator`, `simulation_get_cardinals`, and
 * `simulation_get_distance` functions are defined and accessible within the scope of this
 * function. It also assumes the use of appropriate variables (`distmin`, `dist`, `friend`,
 * `i`, and `j`) and appropriate data types (`INT_MAX` for `distmin`).
 */
void simulation_get_closest_friend(entity_t * predators)
{
    int distmin = INT_MAX;
    int dist = 0;
    int friend = 0;
    int i, j;

    for (i=0;i<NB_PREDATOR;i++){
        for(j=0;j<NB_PREDATOR;j++)
        {

            if(i!=j)
            {
                dist = simulation_get_distance_between_2_predator(predators[i], predators[j]);
                if(dist < distmin)
                {
                    distmin=dist;
                    friend=j;
                }
            }
        }
        predators[i].p.cardinality_friend =
            simulation_get_cardinals(predators[i].x, predators[i].y, predators[friend].x, predators[friend].y);
        predators[i].p.distance_friend =
            simulation_get_distance(distmin, 2*COM_RADIUS);
    }
}


/**
 * @brief Determines the distance category based on the given distance.
 *
 * This function determines the distance category (CLOSE or FAR) based on the given distance
 * (`dsrc`). If the distance is less than or equal to the communication radius (`COM_RADIUS`),
 * it assigns the `CLOSE` value to the `dist` variable. Otherwise, it assigns the `FAR` value
 * to the `dist` variable. The function then returns the `dist` value.
 *
 * @param dsrc The distance to be categorized.
 *
 * @return The distance category (CLOSE or FAR) based on the given distance.
 *
 * @note The function assumes that the `distance` enum and the `COM_RADIUS` constant are
 * defined and accessible within the scope of this function.
 */
enum distance simulation_get_distance(int dsrc, int radius)
{
    enum distance dist = FAR;
    if (dsrc <= radius) dist = CLOSE;
    return dist;
}

/**
 * @brief Determines the perception of each predator in the simulation.
 *
 * This function determines the perception of each predator in the simulation by calling
 * the `simulation_get_closest_friend` function and updating the `cardinality_target` and
 * `distance_target` based on the presence of a target and its distance. For each predator,
 * it checks if the `cardinality_target` is not equal to `NOT_FOUND` or if the distance to
 * the target (`abs(predators[i].x - target.x) + abs(predators[i].y - target.y)`) is within
 * the "CLOSE" range. If either of these conditions is met, it updates the `cardinality_target`
 * with the cardinality (direction) towards the target and the `distance_target` with the
 * calculated distance. This process is performed for all predators in the simulation.
 *
 * @param predators An array of simulation entities representing all predators in the simulation.
 *
 * @note The function assumes that the `NB_PREDATOR`, `simulation_get_closest_friend`,
 * `simulation_get_cardinals`, `simulation_get_distance`, and `target` (global) are defined
 * and accessible within the scope of this function.
 */
void simulation_get_perception(entity_t * predators, target_t target)
{
    int i;
    
    simulation_get_closest_friend(predators);
    for(i=0;i<NB_PREDATOR;i++)
    {
        if(predators[i].p.cardinality_target != NOT_FOUND
           || simulation_get_distance(abs(predators[i].x - target.x) + abs(predators[i].y - target.y), 2*COM_RADIUS) == CLOSE)
        {
            predators[i].p.cardinality_target =
                simulation_get_cardinals(predators[i].x, predators[i].y, target.x, target.y);
            predators[i].p.distance_target =
                simulation_get_distance(abs(predators[i].x - target.x) + abs(predators[i].y - target.y), DESTROY_RADIUS);
        }
    }
}


/**
 * @brief Calculates the distance between two predators in the simulation.
 *
 * This function calculates the distance between two predators in the simulation by
 * subtracting the x-coordinates and y-coordinates of the predators (`p1` and `p2`),
 * taking their absolute values, and summing them. The resulting distance is returned.
 *
 * @param p1 The simulation entity representing the first predator.
 * @param p2 The simulation entity representing the second predator.
 *
 * @return The distance between the two predators.
 */
int simulation_get_distance_between_2_predator(entity_t p1, entity_t p2)
{
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

/* ------- REGLES --------- */
/**
 * @brief Verifies if a predator's perception matches the given rules.
 *
 * This function verifies if a predator's perception matches the given rules by comparing
 * the predator's perception attributes (`distance_friend`, `distance_target`,
 * `cardinality_friend`, and `cardinality_target`) with the corresponding attributes in
 * the given rules (`rule.perception`). If a perception attribute in the rules is not a
 * wildcard (`JOKER_D` or `JOKER_C`), and it doesn't match the predator's perception,
 * the flag is set to 0. The final flag value is returned to indicate if the predator's
 * perception matches the rules.
 *
 * @param predators The simulation entity representing the predator.
 * @param rule      The rules specifying the desired perception attributes.
 *
 * @return 1 if the predator's perception matches the rules, 0 otherwise.
 */

int simulation_verify_rules(entity_t predator,rules_t rule)
{
    int flag=1;
    /*test distance friend*/
    if(rule.perception.distance_friend != JOKER_D
       && predator.p.distance_friend != rule.perception.distance_friend) flag=0;
   
    /*test distance target*/
    if(rule.perception.distance_target != JOKER_D
       && predator.p.distance_target != rule.perception.distance_target) flag=0;
    
    /*test cardinality friend*/
    if(rule.perception.cardinality_friend != JOKER_C
       && predator.p.cardinality_friend != rule.perception.cardinality_friend) flag=0;
    
    /*test direction target*/
    if(rule.perception.cardinality_target != JOKER_C
       && predator.p.cardinality_target != rule.perception.cardinality_target) flag=0;

    return(flag);
}


/**
 * @brief Filters the rules based on a predator's perception.
 *
 * This function filters the rules based on a predator's perception by calling the
 * `simulation_verify_rules` function for each rule in the `brain` array. For each rule,
 * it verifies if the predator's perception matches the rule using `simulation_verify_rules`.
 * If the match is successful, the corresponding index in the `filtered_rules` array is set
 * to 1, indicating that the rule is filtered. Otherwise, the index remains unchanged. This
 * process is performed for all rules in the `brain` array.
 *
 * @param predators        The simulation entity representing the predator.
 * @param filtered_rules   An array to store the filtered rules (1 for filtered, 0 for not filtered).
 * @param brain            The array of rules to be filtered.
 *
 * @note The function assumes that the `NB_RULES` and `simulation_verify_rules` functions
 * are defined and accessible within the scope of this function.
 */
int simulation_filtrage_regle(entity_t predator, int filtered_rules[NB_RULES], rules_t brain[NB_RULES])
{
    int i;
    int nb_compatible=0;
    for(i=0;i<NB_RULES;i++){
        if (simulation_verify_rules(predator, brain[i]))
        {
            filtered_rules[nb_compatible] = i;
            nb_compatible++;
        }
    }
    return(nb_compatible);
}

/**
 * @brief Chooses an action based on the filtered rules.
 *
 * This function chooses an action based on the filtered rules by calculating the probabilities
 * for each filtered rule and using a cumulative probability approach. It first calculates the
 * sum of the priority values raised to the power of `S_POWER` for all filtered rules. Then, it
 * calculates the probability for each filtered rule by dividing the priority value raised to
 * the power of `S_POWER` by the sum. Next, it iterates through the filtered rules and accumulates
 * the probabilities until the cumulative probability exceeds a randomly generated value. Finally,
 * it returns the action corresponding to the rule for which the cumulative probability threshold
 * is exceeded.
 *
 * @param filtered_rules An array indicating which rules are filtered (1 for filtered, 0 for not filtered).
 * @param brain          The array of filtered rules.
 *
 * @return The chosen action based on the filtered rules.
 *
 * @note The function assumes that the `NB_RULES`, `S_POWER`, and `rand` function are defined and
 * accessible within the scope of this function.
 */
int simulation_choose_action(int filtered_rules[NB_RULES], rules_t  brain[NB_RULES], int nb_compatible)
{
    float sum=0.0;
    float cumulativeProbability=0.0;
    float res_pow = 0.0;
    int action=-1;
    int j;
    float p=(float) rand()/(RAND_MAX) ;
    float probability[NB_RULES]={0};

    utils_shuffle(filtered_rules,nb_compatible);
    for(j=0;j<nb_compatible;j++)
    {
        res_pow = powf(brain[filtered_rules[j]].priority, S_POWER);
        sum += res_pow;
        probability[filtered_rules[j]] = res_pow;
    }
    
    for(j=0;j<nb_compatible;j++) {
            cumulativeProbability += probability[filtered_rules[j]]/sum;
            if(p < cumulativeProbability){
                action = brain[filtered_rules[j]].action;
                break;
            }
    }
    return(action);
}


/**
 * @brief Executes the specified action for the predator in the simulation.
 *
 * This function executes the specified action for the predator in the simulation by
 * performing the corresponding operation based on the action value. The possible actions
 * and their corresponding operations are as follows:
 *
 *   - Action 0: Move the predator to the north direction.
 *   - Action 1: Move the predator to the south direction.
 *   - Action 2: Move the predator to the east direction.
 *   - Action 3: Move the predator to the west direction.
 *   - Action 4: Communicate with other predators in the simulation.
 *   - Action 5: Destroy the target entity (if the predator is close to it).
 *
 * @param predator   A pointer to the simulation entity representing the predator.
 * @param action     The action to be executed.
 * @param predators  An array of simulation entities representing all predators in the simulation.
 *
 * @note The function assumes that the `NORTH`, `SOUTH`, `EAST`, `WEST`, `simulation_move_entity`,
 * `simulation_communicate`, and `simulation_destroy_target` functions are defined and accessible
 * within the scope of this function. It also assumes the availability of `NB_PREDATOR` constant.
 */

void simulation_execute_action(entity_t * predator,
                               int action,
                               entity_t predators[NB_PREDATOR],
                               target_t * target)
{
    switch(action)
    {
    case 0:
        simulation_move_entity(predator,NORTH);
        break;
    case 1:
        simulation_move_entity(predator,SOUTH);
        break;
    case 2:
        simulation_move_entity(predator,EAST);
        break;
    case 3:
        simulation_move_entity(predator,WEST);
        break;
    case 4:
        simulation_communicate(*predator, predators);
        break;
    case 5:
        simulation_destroy_target(*predator, target);
        break;
    }
}

/**
 * @brief Initializes the simulation with initial values for the predators and target entity.
 *
 * This function initializes the simulation by setting initial values for the predators and
 * the target entity. The target entity's coordinates are set to (50, 50), and its initial
 * health points are set to `TARGET_PV`. For each predator in the `predators` array, the
 * coordinates are set to (0, 0), the distance to friend and distance to target are set to
 * `FAR`, and the cardinality of friend and cardinality of target are set to `NORTH` and
 * `NOT_FOUND`, respectively. The first predator in the array is also set to coordinates (0, 0).
 *
 * @param predators An array of simulation entities representing the predators.
 *
 * @note The function assumes the availability of `NB_PREDATOR`, `WORLD_WIDTH`, `WORLD_HEIGHT`,
 * `TARGET_PV`, `FAR`, `NORTH`, and `NOT_FOUND` constants, as well as the `rand` function, within
 * the scope of this function.
 */
void simulation_init(entity_t predators[NB_PREDATOR], target_t * target)
{
    int i;
    entity_initialize_target(target);
    for(i=0;i<NB_PREDATOR;i++)
    {
        /* entity_initialize(&predators[i], WORLD_WIDTH/2, WORLD_HEIGHT/2, NULL); */
        /* entity_even_distribution_init(&predators[i], i, NULL); */
        /* entity_all_centered_distribution_init(&predators[i], NULL); */
        /* entity_random_distribution_init(&predators[i], NULL); */
        /* entity_horizontal_distribution_init(&predators[i], NULL); */
        /* entity_vertical_distribution_init(&predators[i], NULL); */
        entity_vertical_even_distribution_init(&predators[i], i, NULL);
    }
}


/**
 * @brief Main simulation loop.
 *
 * This function runs the main simulation loop where the predators and target interact based on the specified brain rules.
 *
 * @param brain The array of rules representing the brain.
 * @param iter  Pointer to the iteration counter.
 */
void simulation_loop(rules_t brain[NB_RULES], int * iter)
{
    
    int action[NB_RULES]={0};
    int filtered_rules[NB_RULES] = {0};
    entity_t predators[NB_PREDATOR];
    target_t target;
    int i, j;
    int nb_compatible=0;
    *iter=0;
    simulation_init(predators, &target);
    
    while(target.pv > 0 && *iter < ITER_MAX)
    {
        (*iter) ++;
        simulation_get_perception(predators, target);

        for(i=0;i<NB_PREDATOR;i++)
        {
            /*init filtered_rules*/
            for(j=0;j<NB_RULES;j++) filtered_rules[j]=0;
            /* filter rules */
            nb_compatible=simulation_filtrage_regle(predators[i], filtered_rules, brain);
            
            /* choisis une action */
            action[i] = simulation_choose_action(filtered_rules,brain,nb_compatible);
            
        }
        /* execute action */
        for(i=0;i<NB_PREDATOR;i++) simulation_execute_action(&predators[i], action[i], predators, &target);
    }
}

/**
 * @brief Run multiple simulations and calculate the average score.
 *
 * This function runs multiple simulations using the specified brain rules and calculates the average score achieved.
 *
 * @param brain The array of rules representing the brain.
 * @param iter  Pointer to store the average score.
 */
void simulation_loop_average(rules_t brain[NB_RULES], int * iter)
{
    int sum =0, score;
    int i;
    for (i = 0; i < NB_SIMULATION_LOOP; ++i) {
	 simulation_loop(brain, &score);
	 sum += score;
    }
    *iter = (int) sum/NB_SIMULATION_LOOP;
}
