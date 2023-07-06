#include "animation.h"
#include "../log/log.h"

#include <dirent.h>
#include <stdio.h>

/**
 * @brief Changes the looping behavior of an animation.
 *
 * This function changes the looping behavior of the specified animation by setting
 * its `loop` property to the specified value. If `is_looping` is non-zero, the animation
 * will loop; otherwise, it will play only once.
 *
 * @param a           A pointer to the pointer of the animation structure.
 * @param is_looping  The value indicating whether the animation should loop (non-zero) or play once (zero).
 */
void animation_change_loop(struct animation_s ** a, int is_looping)
{
    (*a)->loop = is_looping;
}

/**
 * @brief Changes the animation of a sprite to the specified animation.
 *
 * This function changes the animation of the specified sprite to the provided animation.
 * It updates the sprite's animation properties, including the animation array, number of
 * animations, current animation index, and looping behavior. If the sprite's current animation
 * has an existing animation array, it is freed before assigning the new animation array.
 *
 * @param s  A pointer to the sprite structure.
 * @param a  A pointer to the animation structure representing the new animation.
 */
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

/**
 * @brief Frees the memory allocated for a background structure and its associated resources.
 *
 * This function frees the memory allocated for a background structure and its associated resources,
 * including the background's name and texture. It checks if the background pointer is not NULL before
 * performing the deallocation. After freeing the resources, the background structure itself is also freed.
 *
 * @param b  A pointer to the background structure to be freed.
 */
void animation_free_background(struct background_s * b)
{
    if (b)
    {
        if (b->name) free(b->name);
        if (b->t) SDL_DestroyTexture(b->t);
        free(b);
    }
}

/**
 * @brief Frees the memory allocated for a sprite structure and its associated resources.
 *
 * This function frees the memory allocated for a sprite structure and its associated resources,
 * including the animation array, animation structure, rotation data, sprite name, and texture.
 * It checks if the sprite pointer is not NULL before performing the deallocation. After freeing
 * the resources, the sprite structure itself is also freed.
 *
 * @param s  A pointer to the sprite structure to be freed.
 */
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

/**
 * @brief Renders a sprite on the specified renderer at the given destination.
 *
 * This function renders a sprite on the specified SDL renderer at the provided destination.
 * It uses the sprite's texture and the current animation index to determine which portion of
 * the texture should be rendered. The rendered sprite is stretched to fit the specified destination
 * rectangle.
 *
 * @param renderer  The SDL renderer on which to render the sprite.
 * @param s         A pointer to the sprite structure to be rendered.
 * @param dest      The destination rectangle defining where the sprite should be rendered.
 */
void animation_render_sprite(SDL_Renderer * renderer,
                             struct sprite_s * s, SDL_Rect dest)
{
    /* sdl_render_image(renderer, s->t, s->r[s->a->array[(int) s->a->current_animation]]); */
    SDL_RenderCopy(renderer,
                   s->t,
                   &s->r[s->a->array[(int) s->a->current_animation]],
                   &dest);
}


/**
 * @brief Updates the current animation of a sprite and returns its animation state.
 *
 * This function updates the current animation of the specified sprite by incrementing it
 * based on the provided speed. It checks if the current animation index exceeds the total
 * number of animations. If the animation is set to loop, the current animation index is reset
 * to 0. Otherwise, it is set to the last animation index. The function returns the animation
 * state, indicating whether the sprite is still in an animation (1) or has reached the end (0).
 *
 * @param s      A pointer to the pointer of the sprite structure.
 * @param speed  The speed at which to update the current animation.
 * @return       The animation state, indicating whether the sprite is still in an animation (1) or has reached the end (0).
 */
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

/**
 * @brief Creates a reverse animation with the specified number of frames.
 *
 * This function creates a reverse animation with the specified number of frames. It allocates
 * memory for the animation structure and initializes its properties, including the current
 * animation index, total number of animations, looping behavior, and animation array. The
 * animation array is populated in reverse order, starting from the last frame index and
 * decrementing towards the first frame index. The function returns a pointer to the created
 * animation structure.
 *
 * @param n            The number of frames in the reverse animation.
 * @param is_looping   The value indicating whether the animation should loop (non-zero) or play once (zero).
 * @return             A pointer to the created reverse animation structure.
 */
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

/**
 * @brief Creates an animation with the specified number of frames.
 *
 * This function creates an animation with the specified number of frames. It allocates memory
 * for the animation structure and initializes its properties, including the current animation
 * index, total number of animations, looping behavior, and animation array. The animation array
 * is populated with consecutive frame indices from 0 to n-1. The function returns a pointer to
 * the created animation structure.
 *
 * @param n            The number of frames in the animation.
 * @param is_looping   The value indicating whether the animation should loop (non-zero) or play once (zero).
 * @return             A pointer to the created animation structure.
 */
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

/**
 * @brief Creates a sprite from a spritesheet image file.
 *
 * This function creates a sprite from a spritesheet image file. It loads the texture using the provided
 * SDL renderer and initializes the sprite's properties, including the sprite's rectangles for each frame
 * in the spritesheet, the sprite's name, texture, and animation. The function calculates the dimensions and
 * offsets of the spritesheet based on the provided number of frames. If the renderer is NULL, a warning message
 * is logged. The function returns a pointer to the created sprite structure.
 *
 * @param renderer    The SDL renderer used to load the spritesheet texture.
 * @param fname       The file name of the spritesheet image file.
 * @param nb_frame    The number of frames in the spritesheet.
 * @return            A pointer to the created sprite structure.
 */
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
    strncpy(s->name, fname, n_fname-1);

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
    s->n = i;

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

/**
 * @brief Renders a background on the specified renderer across the screen width.
 *
 * This function renders a background on the specified SDL renderer across the width of the screen.
 * It uses the background's texture and source rectangle to determine the portion of the texture to render.
 * The background is repeated horizontally until it covers the entire screen width. The screen height
 * parameter is ignored in this function. The rendering is performed using the provided renderer and
 * destination rectangle.
 *
 * @param renderer    The SDL renderer on which to render the background.
 * @param b           A pointer to the background structure representing the background to render.
 * @param screen_w    The width of the screen.
 * @param screen_h    The height of the screen (unused in this function).
 */
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
