#ifndef _const_h_
#define _const_h_

/* game const */

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define GAME_DELAY 10

#define NB_PREDATOR 9
#define NB_PREY 20
#define NB_GAME_BACKGROUNDS 3

#define PREDATOR_SPEED 20
#define PREDATOR_DAMAGE 10
#define PREY_SPEED 0

#define COM_RADIUS 150
#define DESTROY_RADIUS 50

/* animation const */

#define MAX_PATH 2048
#define MAX_TEXTURE 20

/* simulation */

#define WORLD_WIDTH SCREEN_WIDTH
#define WORLD_HEIGHT SCREEN_HEIGHT
#define S_POWER 5

#define TARGET_PV 1000
#define ITER_MAX 3000

#define NB_SIMULATION_LOOP 50

/* RULES */
#define NB_ACTION      6
#define NB_PRIORITY    5
#define NB_DISTANCE    3
#define NB_CARDINALITY 6
#define NB_MEASURE     5   // size of measures
#define NB_RULES       26  // number rules in brain (size of array_rules)

/* RECUIS */
#define EPSILON  0.001

#endif
