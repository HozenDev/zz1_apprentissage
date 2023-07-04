#ifndef _game_h_
#define _game_h_

#include "../sdl/sdl.h"
#include "../log/log.h"
#include "../utils/utils.h"
#include "../animation/animation.h"
#include "../entity/entity.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define GAME_DELAY 20

#define NB_PREDATOR 8
#define NB_PREY 1
#define NB_GAME_BACKGROUNDS 3

#define PREDATOR_SPEED 2
#define PREY_SPEED 0

struct sprites_available_s {
    char * fish;
    char * jellyfish;
};
enum distance{
    close,
    far
};
enum direction_friend{
    north,
    south,
    east,
    west
};
enum direction_target{
    not_found,
    north,
    south,
    east,
    west,

}
extern const struct sprites_available_s sprites_available;

struct game_state_s {
    int mx;
    int my;

    int running;
    int end;

    SDL_Event event;

    struct background_s ** back;
    int nb_background;

    struct entity_s ** predators;
    int nb_predator;

    struct entity_s * prey;

    float score;
    float time;
    float delay;

    SDL_Rect game_rect;
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
int game_loop(void);

#endif
