#ifndef _attack_h_
#define _attack_h_

#include "../animation/animation.h"

#define MAX_ATTACK 30

struct attack_s {
    struct sprite_s * s;
    SDL_Rect r;
    float damages;
};

typedef struct attack_s attack_t;

struct attack_s * attack_create(SDL_Renderer * renderer, char * fname, int nb_frame, int damages);
void attack_free(struct attack_s * a);

#endif
