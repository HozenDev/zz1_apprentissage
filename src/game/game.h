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
    int stop;
    int print_radius;

    SDL_Event event;

    struct background_s ** back;
    int nb_background;

    /* game simulation state */
    int nb_predator;
    struct entity_s predators[NB_PREDATOR];
    int nb_prey;
    struct target_s prey[NB_PREY];

    int action[NB_PREDATOR];
    int filtered_rules[NB_RULES];
    rules_t brain[NB_RULES];


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
    TTF_Font * small_font;
    TTF_Font * big_font;

    struct game_state_s state;
};

typedef struct game_s game_t;

void game_loop_state_update(game_state_t * g_state);
void game_keyboard_state_update(game_state_t * g_state);
void game_mouse_state_update(game_state_t * g_state);
void game_state_reset(game_state_t * g_state);

void game_graphic_update(game_t game);

int game_initialisation(game_t ** game);
void game_free_game(game_t * game);

int game_loop(rules_t brain[NB_RULES], int * iter);

#endif
