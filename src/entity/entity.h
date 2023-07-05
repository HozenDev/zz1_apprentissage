#ifndef _entity_h_
#define _entity_h_

#include "../animation/animation.h"
#include "../rules/rules.h"

enum entity_sdl_state_e {
    IDLE,
    WALK,
    ATTACK,
    HURT,
    DEATH 
};
typedef enum entity_sdl_state_e entity_sdl_state_t;

struct entity_sdl_s {
    struct sprite_s ** sprites; /* tableau de sprites */
    int n_of_sprites;           /* taille du tableau de sprites */
    int is_in_animation;        /* 1 si entité en animation 0 sinon */

    float speed;                /* vitesse du sprite */

    SDL_Rect r;                 /* dimension & position */

    enum entity_sdl_state_e state;
};

typedef struct entity_sdl_s entity_sdl_t;

struct entity_s {
    int x;
    int y;
    struct perception_s p;
    struct entity_sdl_s * e_sdl;
};

typedef struct entity_s entity_t;

struct target_s {
    int x;
    int y;
    int pv;
    struct entity_sdl_s * e_sdl;
};

typedef struct target_s target_t;

void entity_initialize(struct entity_s * e);
void entity_initialize_target(struct target_s * t);

int entity_sdl_collide(struct entity_sdl_s * e1, struct entity_sdl_s * e2);
void entity_sdl_update_animation(struct entity_sdl_s * e, float speed);
void entity_sdl_scale(struct entity_sdl_s * e, int scale_factor);
void entity_sdl_change_state(struct entity_sdl_s * e, enum entity_sdl_state_e e_s);
void entity_sdl_free(struct entity_sdl_s * e);
struct entity_sdl_s * entity_sdl_create(SDL_Renderer * renderer, char ** sprite_fnames, int n_of_sprites,
                                        int * n_per_sprites, float speed);

#endif
