#include "entity.h"
#include "../log/log.h"

int entity_collide(struct entity_s * e1, struct entity_s * e2)
{
    SDL_Rect intersection;
    int collision = SDL_IntersectRect(&e1->r, &e2->r, &intersection);
    if (intersection.w*intersection.h < 0.6*e1->r.w*e1->r.h)
    {
        collision = 0;
    }
    return collision;
}

/**
 * @brief Update entity animation
 *
 * @param e entity to update
 * @param speed, speed of the entity
 */
void entity_update_animation(struct entity_s * e, float speed)
{
    e->is_in_animation = animation_update_sprite(&(e->sprites[e->state]), speed);
}

/**
 * @brief Change the state of an entity
 *
 * @param e, entity pointer to modify
 * @param e_s, the new state
 */
void entity_change_state(struct entity_s * e, enum entity_state_e e_s)
{
    if (!e->is_in_animation)
    {
        e->state = e_s;
        e->is_in_animation = 1;
    }
    else {
        zlog(stdout, WARNING, "entity is already in animation", 0);
    }
}

/**
 * @brief Free an entity
 *
 * @param e, entity address to free
 */
void entity_free(struct entity_s * e)
{
    int i;
    
    if (e)
    {
        if (e->name) {free(e->name); e->name = NULL;}
        if (e->sprites)
        {
            for (i = 0; i < e->n_of_sprites; ++i) {
                animation_free_sprite(e->sprites[i]);
            }
            free(e->sprites);
            e->sprites = NULL;
        }

        /* clear entity memory */
        e->r.x = 0; e->r.y = 0; e->r.w = 0; e->r.h = 0;
        e->type = 0;
        e->state = 0;
        e->status = 0;
        e->n_of_sprites = 0;

        free(e);
        e = NULL;
    }
}

/**
 * @brief Scale entity
 *
 * @param e, entity pointer
 * @param scale_factor, scale_factor to resize entity rectangle
 */
void entity_scale(struct entity_s * e, int scale_factor)
{
    e->r.w *= scale_factor;
    e->r.h *= scale_factor;
}

/**
 * @brief Create an entity
 *
 * @param renderer, renderer where the entity will be printed
 * @param sprites_fnames, array of all names of sprites_files
 * @param n_of_sprites number of frame per sprites
 * @param n_per_sprites, array of all number of frame in sprites
 * @param e_name, name of the entity
 * @param e_t, type of the entity
 * @param e_s, status of the entity
 *
 * @return entity pointer created
 */
struct entity_s * entity_create(SDL_Renderer * renderer,
                                char ** sprite_fnames,
                                int n_of_sprites,
                                int * n_per_sprites,
                                char * e_name,
                                enum entity_type_e e_t,
                                enum entity_status_e e_s,
                                int life,
                                float speed)
{
    /* get entity name length */
    int n_name = strlen(e_name);
    int i;

    /* initialize entity */
    struct entity_s * e = (struct entity_s *) malloc(sizeof(struct entity_s));

    if (!e) exit(-1);
    zlog(stdout, INFO, "OK '%s'", "entity is allocated");

    e->sprites = (struct sprite_s **) malloc(sizeof(struct sprite_s *)*n_of_sprites);
    if (!e->sprites) exit(-1);
    zlog(stdout, INFO, "OK '%s'", "entity sprites are allocated");
    
    /* initialize entity's sprites */
    e->n_of_sprites = n_of_sprites;
    for (i = 0; i < n_of_sprites; ++i)
    {
        e->sprites[i] = animation_spritesheet_from_file(renderer, sprite_fnames[i], n_per_sprites[i]);
    }
    
    /* initialize entity name */
    e->name = (char *) malloc(sizeof(char)*n_name);
    strncpy(e->name, e_name, n_name);

    /* initialize entity states */
    e->type = e_t;
    e->status = e_s;
    e->state = IDLE;
    e->is_in_animation = 0;
    e->life = life;
    e->speed = speed;

    /* initialize rect */
    e->r = (SDL_Rect) {.x = 0, .y = 0, .w = e->sprites[0]->r[0].w, .h = e->sprites[0]->r[0].h};

    return e;
}
