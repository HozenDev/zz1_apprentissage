#include "entity.h"
#include "../log/log.h"

int entity_sdl_collide(struct entity_sdl_s * e1, struct entity_sdl_s * e2)
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
void entity_sdl_update_animation(struct entity_sdl_s * e, float speed)
{
    e->is_in_animation = animation_update_sprite(&(e->sprites[e->state]), speed);
}

/**
 * @brief Change the state of an entity
 *
 * @param e, entity pointer to modify
 * @param e_s, the new state
 */
void entity_sdl_change_state(struct entity_sdl_s * e, enum entity_sdl_state_e e_s)
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
void entity_sdl_free(struct entity_sdl_s * e)
{
    int i;
    
    if (e)
    {
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
        e->state = 0;
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
void entity_sdl_scale(struct entity_sdl_s * e, int scale_factor)
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
 * @param speed, speed of the entity
 *
 * @return entity pointer created
 */
struct entity_sdl_s * entity_sdl_create(SDL_Renderer * renderer,
                                        char ** sprite_fnames,
                                        int n_of_sprites,
                                        int * n_per_sprites,
                                        float speed)
{
    int i;

    /* allocation entity */
    struct entity_sdl_s * e = (struct entity_sdl_s *) malloc(sizeof(struct entity_sdl_s));
    e->sprites = (struct sprite_s **) malloc(sizeof(struct sprite_s *)*n_of_sprites);

    if (!e || !e->sprites) {
        zlog(stdout, ERROR, "!! Bad entity allocation", 0);
        exit(-1);
    }

    /* initialize entity's sprites */
    e->n_of_sprites = n_of_sprites;

    /* load all sprites */
    for (i = 0; i < n_of_sprites; ++i) {
        if (renderer) e->sprites[i] =
                          animation_spritesheet_from_file(renderer, sprite_fnames[i], n_per_sprites[i]);
        else e->sprites[i] = NULL;
    }

    if (renderer) /* initialize rect */
        e->r = (SDL_Rect) {.x = 0, .y = 0, .w = e->sprites[0]->r[0].w, .h = e->sprites[0]->r[0].h};

    /* initialize entity states */
    e->state = IDLE;
    e->is_in_animation = 0;
    e->speed = speed;

    return e;
}

void entity_initialize_target(struct target_s * t)
{
    float p = ((float)rand()/RAND_MAX);

    if (p < 0.25)
    {
        t->x = rand()%((int) (0.25*WORLD_WIDTH));
        t->y = rand()%((int) (0.25*WORLD_HEIGHT));
    }
    else if (p < 0.5)
    {
        t->x = rand()%((int) (0.25*WORLD_WIDTH)) + (0.75*WORLD_WIDTH - 40);
        t->y = rand()%((int) (0.25*WORLD_HEIGHT));
    }
    else if (p < 0.75)
    {
        t->x = rand()%((int) (0.25*WORLD_WIDTH));
        t->y = rand()%((int) (0.25*WORLD_HEIGHT)) +(0.75*WORLD_HEIGHT - 40);
    }
    else {
        t->x = rand()%((int) (0.25*WORLD_WIDTH)) + (0.75*WORLD_WIDTH - 40);
        t->y = rand()%((int) (0.25*WORLD_HEIGHT)) + (0.75*WORLD_HEIGHT - 40);
    }
    
    t->pv = TARGET_PV;
}

/**
 * @brief Initialize an entity
 *
 * @param e, entity_s to initialize
 * @param x, coordinate x where entity begin
 * @param y, coordinate x where entity begin
 * @param e_sdl, entity_sdl_s of the entity
 *
 * @note e_sdl can be NULL for simulation
 */
void entity_initialize(struct entity_s * e, int x, int y, entity_sdl_t * e_sdl)
{
    /* initialize coordinates */
    /* e->x = ((float) rand()/RAND_MAX)*WORLD_WIDTH; */
    /* e->y = ((float) rand()/RAND_MAX)*WORLD_HEIGHT; */
    e->x = x;
    e->y = y;

    /* initialize perception */
    /* initialize other predators perceptions */
    e->p.distance_friend = FAR;
    e->p.cardinality_friend = NORTH;

    /* initialize target perceptions */
    e->p.distance_target = FAR;
    e->p.cardinality_target = NOT_FOUND;

    e->e_sdl = e_sdl;
    //if (!e_sdl) zlog(stdout, WARNING, "entity_sdl set to NULL pointer", 0);
}

void entity_even_distribution_init(struct entity_s *e ,int i,entity_sdl_t * e_sdl)
{
    e->x=SCREEN_WIDTH*(i/2+1)/(NB_PREDATOR/2+1);
    e->y=SCREEN_HEIGHT*(3*(i%2)+1)/5;

    e->p.distance_friend = FAR;
    e->p.cardinality_friend = NORTH;

    /* initialize target perceptions */
    e->p.distance_target = FAR;
    e->p.cardinality_target = NOT_FOUND;

    e->e_sdl = e_sdl;
}

void entity_random_distribution_init(struct entity_s *e ,entity_sdl_t * e_sdl)
{
    e->x=(float )(rand())/RAND_MAX*SCREEN_WIDTH;
    e->y=SCREEN_HEIGHT*(float)(rand())/RAND_MAX;
    
    e->p.distance_friend = FAR;
    e->p.cardinality_friend = NORTH;

    /* initialize target perceptions */
    e->p.distance_target = FAR;
    e->p.cardinality_target = NOT_FOUND;

    e->e_sdl = e_sdl;
}

void entity_all_centered_distribution_init(struct entity_s *e ,entity_sdl_t * e_sdl)
{
    e->x=SCREEN_WIDTH/2;
    e->y=SCREEN_HEIGHT/2;
    
    e->p.distance_friend = FAR;
    e->p.cardinality_friend = NORTH;

    /* initialize target perceptions */
    e->p.distance_target = FAR;
    e->p.cardinality_target = NOT_FOUND;

    e->e_sdl = e_sdl;
}

void entity_horizontal_distribution_init(struct entity_s *e ,entity_sdl_t * e_sdl)
{
    e->x=(float )(rand())/RAND_MAX*SCREEN_WIDTH;
    e->y=SCREEN_HEIGHT*0.5;
    
    e->p.distance_friend = FAR;
    e->p.cardinality_friend = NORTH;

    /* initialize target perceptions */
    e->p.distance_target = FAR;
    e->p.cardinality_target = NOT_FOUND;

    e->e_sdl = e_sdl;
}

void entity_vertical_even_distribution_init(struct entity_s *e, int i, entity_sdl_t * e_sdl)
{
    e->x = 0;
    e->y = i*SCREEN_HEIGHT/NB_PREDATOR;
    
    e->p.distance_friend = FAR;
    e->p.cardinality_friend = NORTH;

    /* initialize target perceptions */
    e->p.distance_target = FAR;
    e->p.cardinality_target = NOT_FOUND;

    e->e_sdl = e_sdl;
}

void entity_vertical_distribution_init(struct entity_s *e ,entity_sdl_t * e_sdl)
{
    e->x=SCREEN_WIDTH*0.5;
    e->y=SCREEN_HEIGHT*(float)(rand())/RAND_MAX;
    
    e->p.distance_friend = FAR;
    e->p.cardinality_friend = NORTH;

    /* initialize target perceptions */
    e->p.distance_target = FAR;
    e->p.cardinality_target = NOT_FOUND;

    e->e_sdl = e_sdl;
}
