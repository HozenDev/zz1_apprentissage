#ifndef _game_h_
#define _game_h_

#include "../sdl/sdl.h"
#include "../log/log.h"
#include "../utils/utils.h"
#include "../animation/animation.h"
#include "../entity/entity.h"
#include "../const/const.h"
#include "../simulation/simulation.h"
#include "../rules/rules.h"

struct sprites_available_s {
    char * fish;
    char * jellyfish;
};

extern const struct sprites_available_s sprites_available;

struct game_state_s {
    int mx;
    int my;

    int running;
    int end;

    SDL_Event event;

    struct background_s ** back;
    int nb_background;

    /* game simulation state */
    int nb_predator;
    struct entity_s predators[NB_PREDATOR];
    int action[NB_PREDATOR];
    int filtered_rules[NB_RULES];
    rules_t brain[NB_RULES];

    struct target_s prey;

    /* game parameters */
    float score;
    float time;
    float delay;

    SDL_Rect game_rect; /* surface of the game */
};

typedef struct game_state_s game_state_t;  

struct game_s {
    int sw;
    int sh;

    SDL_Renderer * renderer;
    SDL_Window * window;
    TTF_Font * font;

    struct game_state_s state;
};

typedef struct game_s game_t;

SDL_bool game_kill_mole(int x,int y,SDL_Rect mole);
void game_graphic_update(game_t game);
void game_mouse_state_update(game_state_t * g_state);
int game_initialisation(game_t ** game);
int game_loop(rules_t brain[NB_RULES]);

#endif
