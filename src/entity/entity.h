#ifndef _entity_h_
#define _entity_h_

#include "../animation/animation.h"

enum entity_type_e {
    PLAYER, IA
};

enum entity_state_e {
    IDLE,
    WALK,
    ATTACK,
    HURT,
    DEATH 
};

enum entity_status_e {
    GOOD,
    BAD,
    NEUTRAL
};

struct entity_s {
    char * name;
    
    struct sprite_s ** sprites;
    int n_of_sprites;
    int is_in_animation;

    float speed;
    int life;

    SDL_Rect r;

    enum entity_type_e type;
    enum entity_state_e state;
    enum entity_status_e status;
};

typedef struct entity_s entity_t;

struct entity_s * entity_create(SDL_Renderer * renderer,
                                char ** sprite_fnames,
                                int n_of_sprites,
                                int * n_per_sprites,
                                char * e_name,
                                enum entity_type_e e_t,
                                enum entity_status_e e_s,
                                int life,
                                float speed);

int entity_collide(struct entity_s * e1, struct entity_s * e2);
void entity_update_animation(struct entity_s * e, float speed);
void entity_scale(struct entity_s * e, int scale_factor);
void entity_free(struct entity_s * e);
void entity_change_state(struct entity_s * e, enum entity_state_e e_s);

#endif
