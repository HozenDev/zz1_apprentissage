#include "attack.h"

/**
 * @brief Create an attack
 *
 * @param renderer, renderer where sprites will be render
 * @param fname, file name
 * @param nb_frame, number of frame in the sprite
 * @param damages, attack damages
 *
 * @return a, struct attack_s pointer
 */
struct attack_s * attack_create(SDL_Renderer * renderer, char * fname, int nb_frame, int damages)
{
    struct attack_s a = (struct attack_s *) malloc(sizeof(struct attack_s));

    a->s = animation_spritesheet_from_file(renderer, fname, nb_frame);
    a->r.x = 0;
    a->r.y = 0;
    a->r.w = a->s->r[0].w;
    a->r.h = a->s->r[0].h;
    a->damages = damages;
    
    return a;
}

/**
 * @brief Free attack
 * 
 * @param a, struct attack_s pointer
 */
void attack_free(struct attack_s * a)
{
    if (a)
    {
        if (a->s) animation_free_sprite(a->s);
        a->r = (SDL_Rect) {.x = 0, .y = 0, .w = 0, .h = 0};
        a->damages = 0;
    }
}
