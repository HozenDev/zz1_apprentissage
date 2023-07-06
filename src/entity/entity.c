#include "entity.h"
#include "../log/log.h"

/**
 * @brief Checks for collision between two SDL entities.
 *
 * This function checks for collision between two SDL entities represented by the `entity_sdl_s`
 * structures `e1` and `e2`. It uses the SDL_IntersectRect function to determine if there is an
 * intersection between the bounding rectangles of the entities. If the area of intersection is less
 * than 0.6 times the area of `e1`'s bounding rectangle, the collision is considered invalid. The
 * function returns a non-zero value if there is a valid collision, and zero otherwise.
 *
 * @param e1  A pointer to the first entity structure.
 * @param e2  A pointer to the second entity structure.
 * @return    Non-zero value if there is a valid collision, zero otherwise.
 */
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

/**
 * @brief Initializes a target entity with random coordinates within the world boundaries.
 *
 * This function initializes a target entity represented by the `target_s` structure `t` with random
 * coordinates within the boundaries of the world. It generates a random value `p` between 0 and 1 to
 * determine the placement quadrant of the target. Based on the value of `p`, the target's `x` and `y`
 * coordinates are set within the corresponding quadrant of the world. The target's hit points (`pv`) are
 * set to a predefined constant (`TARGET_PV`). 
 *
 * @param t  A pointer to the target entity structure to be initialized.
 */
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


/**
 * @brief Initializes an entity with even distribution coordinates and default perceptions.
 *
 * This function initializes an entity represented by the `entity_s` structure `e` with even distribution
 * coordinates and default perceptions. The `i` parameter is used to calculate the entity's `x` and `y`
 * coordinates based on the screen width and height. The function assigns the `distance_friend` and
 * `cardinality_friend` perceptions to default values. It also initializes the target perceptions by
 * setting the `distance_target` to FAR and `cardinality_target` to NOT_FOUND. The `e_sdl` parameter is
 * assigned to the `e_sdl` member of the entity structure.
 *
 * @param e        A pointer to the entity structure to be initialized.
 * @param i        The index used to calculate the entity's coordinates.
 * @param e_sdl    A pointer to the entity's SDL structure.
 */
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


/**
 * @brief Initializes an entity with random distribution coordinates and default perceptions.
 *
 * This function initializes an entity represented by the `entity_s` structure `e` with random distribution
 * coordinates and default perceptions. The `e` parameter is assigned random `x` and `y` coordinates within
 * the boundaries of the screen. The function sets the `distance_friend` and `cardinality_friend` perceptions
 * to default values. It also initializes the target perceptions by setting the `distance_target` to FAR and
 * `cardinality_target` to NOT_FOUND. The `e_sdl` parameter is assigned to the `e_sdl` member of the entity structure.
 *
 * @param e        A pointer to the entity structure to be initialized.
 * @param e_sdl    A pointer to the entity's SDL structure.
 */
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


/**
 * @brief Initializes an entity with centered distribution coordinates and default perceptions.
 *
 * This function initializes an entity represented by the `entity_s` structure `e` with centered distribution
 * coordinates and default perceptions. The entity's `x` coordinate is set to half of the screen width, and the
 * `y` coordinate is set to half of the screen height. The function sets the `distance_friend` and `cardinality_friend`
 * perceptions to default values. It also initializes the target perceptions by setting the `distance_target` to FAR and
 * `cardinality_target` to NOT_FOUND. The `e_sdl` parameter is assigned to the `e_sdl` member of the entity structure.
 *
 * @param e        A pointer to the entity structure to be initialized.
 * @param e_sdl    A pointer to the entity's SDL structure.
 */
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


/**
 * @brief Initializes an entity with horizontal distribution coordinates and default perceptions.
 *
 * This function initializes an entity represented by the `entity_s` structure `e` with horizontal distribution
 * coordinates and default perceptions. The `e` parameter is assigned a random `x` coordinate within the boundaries
 * of the screen, while the `y` coordinate is set to half of the screen height. The function sets the `distance_friend`
 * and `cardinality_friend` perceptions to default values. It also initializes the target perceptions by setting the
 * `distance_target` to FAR and `cardinality_target` to NOT_FOUND. The `e_sdl` parameter is assigned to the `e_sdl`
 * member of the entity structure.
 *
 * @param e        A pointer to the entity structure to be initialized.
 * @param e_sdl    A pointer to the entity's SDL structure.
 */
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

/**
 * @brief Initializes an entity with vertical distribution coordinates and default perceptions.
 *
 * This function initializes an entity represented by the `entity_s` structure `e` with vertical distribution
 * coordinates and default perceptions. The `x` coordinate is set to half of the screen width, while the `y` coordinate
 * is assigned a random value within the boundaries of the screen. The function sets the `distance_friend` and `cardinality_friend`
 * perceptions to default values. It also initializes the target perceptions by setting the `distance_target` to FAR and
 * `cardinality_target` to NOT_FOUND. The `e_sdl` parameter is assigned to the `e_sdl` member of the entity structure.
 *
 * @param e        A pointer to the entity structure to be initialized.
 * @param e_sdl    A pointer to the entity's SDL structure.
 */
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


/**
 * @brief Initializes the vertical even distribution of an entity.
 *
 * This function sets the x and y coordinates of the entity based on the index i,
 * as well as initializes the distance and cardinality values for friend and target perceptions.
 * Finally, it assigns the entity_sdl_t pointer to the entity.
 *
 * @param e Pointer to the entity structure to be initialized.
 * @param i Index of the entity.
 * @param e_sdl Pointer to the entity_sdl_t structure associated with the entity.
 *
 * @return None.
 */
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

/**
 * @brief Initializes the horizontal even distribution of an entity.
 *
 * This function sets the x and y coordinates of the entity based on the index i,
 * as well as initializes the distance and cardinality values for friend and target perceptions.
 * Finally, it assigns the entity_sdl_t pointer to the entity.
 *
 * @param e Pointer to the entity structure to be initialized.
 * @param i Index of the entity.
 * @param e_sdl Pointer to the entity_sdl_t structure associated with the entity.
 *
 * @return None.
 */
void entity_horizontal_even_distribution_init(struct entity_s *e, int i, entity_sdl_t * e_sdl)
{
    e->x = i*SCREEN_WIDTH/NB_PREDATOR;
    e->y = 0;
    
    e->p.distance_friend = FAR;
    e->p.cardinality_friend = NORTH;

    /* initialize target perceptions */
    e->p.distance_target = FAR;
    e->p.cardinality_target = NOT_FOUND;

    e->e_sdl = e_sdl;
}
