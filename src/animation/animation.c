#include "animation.h"
#include "../log/log.h"

#include <dirent.h>
#include <stdio.h>

void animation_change_loop(struct animation_s ** a, int is_looping)
{
    (*a)->loop = is_looping;
}

void animation_change_animation(struct sprite_s * s, struct animation_s * a)
{
    int i;

    if (s->a->array) free(s->a->array);
    
    s->a->n = a->n;
    s->a->current_animation = a->current_animation;
    s->a->array = (int *) malloc(sizeof(int));
    s->a->loop = a->loop;

    for (i = 0; i < a->n; ++i)
    {
        s->a->array[i] = a->array[i];
    }
}

void animation_free_background(struct background_s * b)
{
    if (b)
    {
        if (b->name) free(b->name);
        if (b->t) SDL_DestroyTexture(b->t);
        free(b);
    }
}

void animation_free_sprite(struct sprite_s * s)
{
    if (s)
    {
        if (s->a && s->a->array) free(s->a->array);
        if (s->a) free(s->a);
        if (s->r) free(s->r);
        if (s->name) free(s->name);
        if (s->t) SDL_DestroyTexture(s->t);
        free(s);
    }
}

void animation_render_sprite(SDL_Renderer * renderer,
                             struct sprite_s * s, SDL_Rect dest)
{
    /* sdl_render_image(renderer, s->t, s->r[s->a->array[(int) s->a->current_animation]]); */
    SDL_RenderCopy(renderer,
                   s->t,
                   &s->r[s->a->array[(int) s->a->current_animation]],
                   &dest);
}

int animation_update_sprite(struct sprite_s ** s, float speed)
{
    int is_in_animation = 1;
    (*s)->a->current_animation += speed;
    if ((int) (*s)->a->current_animation >= (*s)->a->n)
    {
        if ((*s)->a->loop == 1) (*s)->a->current_animation = 0.0;
        else {
            (*s)->a->current_animation = (*s)->a->n - 1;
        }
        is_in_animation = 0;
    }
    return is_in_animation;
}

animation_t * animation_create_animation_reverse(int n, int is_looping)
{
    int i;
    animation_t * a = (animation_t *) malloc(sizeof(animation_t));

    a->current_animation = 0.0;
    a->n = n;
    a->loop = is_looping;
    a->array = (int *) malloc(sizeof(int)*n);

    for (i = 0; i < n; ++i) a->array[i] = n-i-1;

    return a;
}

animation_t * animation_create_animation(int n, int is_looping)
{
    int i;
    animation_t * a = (animation_t *) malloc(sizeof(animation_t));

    a->current_animation = 0.0;
    a->n = n;
    a->array = (int *) malloc(sizeof(int)*n);
    a->loop = is_looping;

    for (i = 0; i < n; ++i) a->array[i] = i;

    return a;
}

struct sprite_s * animation_spritesheet_from_file(SDL_Renderer * renderer, char * fname, int nb_frame)
{
    int i = 0;
    int x, y;
    int height, width;
    int offset_x;
    int offset_y;
    int n_fname = strlen(fname);

    struct sprite_s * s = NULL;
    
    s = (struct sprite_s *) malloc(sizeof(struct sprite_s));
    s->r = (SDL_Rect *) malloc(sizeof(SDL_Rect)*nb_frame);
    s->name = (char *) malloc(sizeof(char)*n_fname);
    s->t = NULL;

    if (!s || !s->r || !s->name)
    {
        zlog(stderr, ERROR, "Error in sprites allocations", 0);
        exit(EXIT_FAILURE);
    }

    /* copie du nom du sprite */
    strncpy(s->name, fname, n_fname);

    if (renderer)
    {
        /* chargement de la texture */
        s->t = sdl_load_image(renderer, fname);

        if (!s->t)
        {
            zlog(stderr, ERROR, "'%s' cannot be loaded", fname);
            exit(-1);
        }
    
        /* initialisation des rectangles du spritesheet */
        SDL_QueryTexture(s->t, NULL, NULL, &width, &height);

        offset_x = width / nb_frame;
        offset_y = height / 1;

        s->n = nb_frame;
        for (y = 0; y < height; y += offset_y)
        {
            for (x = 0; x < width; x += offset_x)
            {
                s->r[i].x = x;
                s->r[i].y = y;
                s->r[i].w = offset_x;
                s->r[i].h = offset_y;
                i++;
            }
        }
    }
    else {
        zlog(stdout, WARNING, "Renderer is NULL", 0);
    }

    /* initialisation de l'animation */
    s->a = animation_create_animation(nb_frame, 1);

    return s;
}

/**
 * @brief load and allocate a background from a file name
 *
 * @param renderer, renderer where the background will be printed
 * @param fname, file name of the background (path)
 *
 * @return
 */
struct background_s * animation_background_from_file(SDL_Renderer * renderer,
                                                     char * fname)
{
    struct background_s * b = NULL;

    b = (struct background_s *) malloc(sizeof(struct background_s));
    b->name = (char *) malloc(sizeof(char)*strlen(fname));

    if (!b || !b->name)
    {
        zlog(stderr, ERROR, "Error in allocation background", 0);
        exit(-1);
    }
    
    strncpy(b->name, fname, strlen(fname)-1);
    b->r.x = 0;
    b->r.y = 0;
    b->t = NULL;

    if (renderer)
    {
        /* chargement de la texture */
        b->t = sdl_load_image(renderer, fname);
        if (!b->t)
        {
            zlog(stderr, ERROR, "Error in allocation background texture", 0);
            exit(EXIT_FAILURE);
        }
        SDL_QueryTexture(b->t, NULL, NULL, &b->r.w, &b->r.h);
    }
    
    return b;
}

void animation_render_background(SDL_Renderer * renderer,
                                 struct background_s * b,
                                 int screen_w, int screen_h)
{
    SDL_Rect r_screen;
    SDL_Rect destRect;

    (void) screen_h;

    r_screen.x = b->r.x;
    r_screen.y = b->r.y;
    r_screen.w = b->r.w;
    r_screen.h = b->r.h;

    while (r_screen.x < screen_w)
    {
        destRect.x = r_screen.x;
        destRect.y = 0;
        destRect.w = b->r.w;
        destRect.h = b->r.h;
        SDL_RenderCopy(renderer, b->t, NULL, &destRect);
        r_screen.x += b->r.w;
    }
}
