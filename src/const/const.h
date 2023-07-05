#ifndef _const_h_
#define _const_h_

/* game const */

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define GAME_DELAY 10

#define NB_PREDATOR 8
#define NB_PREY 1
#define NB_GAME_BACKGROUNDS 3

#define PREDATOR_SPEED 3
#define PREDATOR_DAMAGE 1
#define PREY_SPEED 0

#define COM_RADIUS 50

/* animation const */

#define MAX_PATH 2048
#define MAX_TEXTURE 20

/* simulation */

#define WORLD_WIDTH SCREEN_WIDTH
#define WORLD_HEIGHT SCREEN_HEIGHT
#define S_POWER 0.1

#define TARGET_PV 1000
#define ITER_MAX 50000

#define NB_SIMULATION_LOOP 4

/* RULES */
#define NB_ACTION      6
#define NB_PRIORITY    3
#define NB_DISTANCE    3
#define NB_CARDINALITY 6
#define NB_MEASURE     4   // size of measures
#define NB_RULES       13  // number rules in brain (size of array_rules)


/*RECUIS*/
#define EPSILON  0.001

#endif
