#include "game.h"

const struct sprites_available_s sprites_available = 
{
    .fish = "fish",
    .turtle = "turtle"
};

/**
 * @brief update game state when keyboard pressed
 *
 * @param g_state, game state pointer
 */
void game_keyboard_state_update(game_state_t * g_state)
{
    switch (g_state->event.key.keysym.sym)
    {
    case SDLK_SPACE:
        g_state->entities[0]->is_in_animation = 0;
        if (g_state->entities[0]->state != ATTACK) entity_change_state(g_state->entities[0], ATTACK);
        break;
    case SDLK_DOWN:
        if (g_state->entities[0]->state != WALK) entity_change_state(g_state->entities[0], WALK);
        break;
    case SDLK_UP:
        if (g_state->entities[0]->state != WALK) entity_change_state(g_state->entities[0], WALK);
        break;
    case SDLK_LEFT:
        if (g_state->entities[0]->state != WALK) entity_change_state(g_state->entities[0], WALK);
        break;
    case SDLK_RIGHT:
        if (g_state->entities[0]->state != WALK) entity_change_state(g_state->entities[0], WALK);
        break;
    default:
        /* todo */
    }
}

/**
 * @brief updates game state on each loop turn
 * 
 * @param g_state, game state pointer
 */
void game_loop_state_update(game_state_t * g_state)
{
    int i;
    /* animation_t * new_a = NULL; */
    
    for (i = 0; i < g_state->nb_background; ++i)
    {
        g_state->back[i]->r.x -= (i+1);
    }

    for (i = 1; i < g_state->nb_entities; ++i)
    {
        if (entity_collide(g_state->entities[0], g_state->entities[i]))
        {
            if (g_state->entities[0]->state == ATTACK)
            {
                if (g_state->entities[i]->state != HURT)
                {
                    g_state->entities[i]->is_in_animation = 0;
                    entity_change_state(g_state->entities[i], HURT);
                    g_state->score += 1;
                    g_state->entities[i]->life -= 1;
                    if (g_state->entities[i]->life < 0) g_state->entities[i]->life = 0;
                }
            }
            else {
                g_state->entities[0]->is_in_animation = 0;
                entity_change_state(g_state->entities[0], HURT);
                g_state->entities[0]->life -= 1;
                if (g_state->entities[0]->life < 0) g_state->entities[0]->life = 0;
                break;
            }
        }
    }
    
    /* update entity animations */
    for (i = 0; i < g_state->nb_entities; ++i)
    {
        /* animation_update_sprite(&(g_state->entities[i]->sprites[g_state->entities[i]->state]), 0.2); */
        if (g_state->entities[i]->life == 0 && g_state->entities[0]->is_in_animation == 0)
        {
            /* todo */
        }
        else {
            entity_update_animation(g_state->entities[i], 0.2);
        }
    }

    g_state->old_frame_time = g_state->new_frame_time;
    g_state->new_frame_time = SDL_GetTicks();
    if(g_state->new_frame_time > 0)
    {
        g_state->fps = 1 / ((float) (g_state->new_frame_time-g_state->old_frame_time)/1000);
    }

    if (!g_state->end) {
        if ( g_state->keystate[SDL_SCANCODE_RIGHT] && g_state->entities[0]->r.x < SCREEN_WIDTH-g_state->entities[0]->r.w)
            g_state->entities[0]->r.x += HERO_SPEED;
        if ( g_state->keystate[SDL_SCANCODE_LEFT] && g_state->entities[0]->r.x > 0)
            g_state->entities[0]->r.x -= HERO_SPEED; 
        if ( g_state->keystate[SDL_SCANCODE_UP] && g_state->entities[0]->r.y > 0)
            g_state->entities[0]->r.y -= HERO_SPEED;
        if ( g_state->keystate[SDL_SCANCODE_DOWN] && g_state->entities[0]->r.y < SCREEN_HEIGHT-g_state->entities[0]->r.h)
            g_state->entities[0]->r.y += HERO_SPEED;

        for (i = 0; i < g_state->nb_entities; ++i)
        {
            if (!g_state->entities[i]->is_in_animation)
            {
                entity_change_state(g_state->entities[i], IDLE);
            }
            if (g_state->entities[i]->life <= 0)
            {
                g_state->entities[i]->is_in_animation = 0;
                entity_change_state(g_state->entities[i], DEATH);
            }
        }

        if (g_state->entities[0]->life == 0)
        {
            g_state->end = 1;
        }
    }
    else {
        /* gestion fin de jeu */
    }
}

/**
 * @brief Free a game
 *
 * @param game, the game struct to be cleared
 */
void game_free_game(game_t * game)
{
    int i;
    
    if (game)
    {
        /* free window and game renderer */
        if (game->window) {SDL_DestroyWindow(game->window); game->window = NULL;}
        if (game->renderer) {SDL_DestroyRenderer(game->renderer); game->renderer = NULL;}

        /* clear game attributs */
        game->sw = 0;
        game->sh = 0;

        /* clear state */
        game->state.mx = 0;
        game->state.my = 0;

        for (i = 0; i < game->state.nb_entities; ++i)
        {
            entity_free(game->state.entities[i]);
        }
        free(game->state.entities);

        for (i = 0; i < game->state.nb_background; ++i)
        {
            animation_free_background(game->state.back[i]);
        }
        free(game->state.back);

        free(game);
    }
}

/**
 * @brief Update game graphic
 * 
 * @param game_t to update graphic
 */
void game_graphic_update(game_t game)
{
    /* todo : update graphic */
    char buf[2048];
    int i;

    /* parallax background */
    for (i = 0; i < game.state.nb_background-1; ++i)
    {
        animation_render_background(game.renderer, game.state.back[i], game.sw, game.sh);
    }

    /* render all sprites */
    for (i = 0; i < game.state.nb_entities; ++i)
    {
        animation_render_sprite(game.renderer,
                                game.state.entities[i]->sprites[game.state.entities[i]->state],
                                game.state.entities[i]->r);
    }
    
    animation_render_background(game.renderer, game.state.back[game.state.nb_background-1], game.sw, game.sh);
    /* sdl_render_image(game.renderer, game.back->t, game.back->r); */

    if (game.state.end) { /* écran de fin de jeu */

        /* screen of end */
        /* sdl_set_renderer_color(game.renderer, (SDL_Color) {.r=0, .g=0, .b=0, .a=150}); */
        /* sdl_draw_rect_coords(game.renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); */

        /* print a text */
        /* sdl_print_text(game.window, game.renderer, game.font, "Jouer", */
        /*                (SDL_Point) {.x = -1, .y = 200}, colors_available.WHITE); */
    }
    
    /* print score */
    sprintf(buf, "Score: %d", (int) game.state.score);
    sdl_print_text(game.window, game.renderer, game.font, buf,
                   (SDL_Point) {.x = 10, .y = 10}, colors_available.BLACK);
}

/**
 * @brief Reset a game state
 *
 * @param g_state, game state pointer
 */
void game_state_reset(game_state_t * g_state)
{
    int i;
    /* animation_t * new_a = NULL; */
    
    g_state->running = 1;
    g_state->end = 0;

    g_state->score = 0.0;
    g_state->delay = GAME_DELAY;
    
    /* reset all sprites */
    for (i = 0; i < g_state->nb_entities; ++i)
    {
        /* new_a = animation_create_animation(g_state->entities[i]->sprites[i]->a->n, 1); */
        /* g_state->sprites[i]->a->current_animation = 0.0; */
        /* g_state->sprites[i]->d.x = 0; */
        /* g_state->sprites[i]->d.y = 0; */
        /* animation_change_animation(g_state->sprites[i], new_a); */
        /* free(new_a); */
    }
}

/**
 * @brief Update a game state
 *
 * @param g_state, the game's state to be updated
 */
void game_mouse_state_update(game_state_t * g_state)
{
    /* int i; */

    if (!g_state->end) {
        switch (g_state->event.button.button)
        {
        case SDL_BUTTON_LEFT:
            /* todo: changement état après click souris gauche */
            break;
        case SDL_BUTTON_RIGHT:
            /* todo: changement état après click souris droit */
            break;
        default:
            break;
        }
    }
    else {
        /* gestion fin de jeu */
    }
}

/**
 * @brief initialise a game_t structure
 *
 * @param game, game_t to be initialized
 *
 * @return exit code, 0 success, 1 failure
 */
int game_initialisation(game_t ** game)
{
    int i;
    int n_sp = 5;
    char ** e_fnames = (char **) malloc(sizeof(char *)*n_sp);
    int * tab = (int *) malloc(sizeof(tab)*n_sp);
    
    (*game) = (game_t *) malloc(sizeof(game_t));
    
    (*game)->sw = SCREEN_WIDTH;
    (*game)->sh = SCREEN_HEIGHT;

    /* initialize (*game) state */
    (*game)->state.mx = 0;
    (*game)->state.my = 0;
    (*game)->state.running = 1;
    (*game)->state.end = 0;
    (*game)->state.score = 0;

    (*game)->state.fps = 0;
    (*game)->state.old_frame_time = 0;
    (*game)->state.new_frame_time = 0;

    /* ------ initialisation SDL2 --------- */
    
    /* SDL initialisation */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        zlog(stderr, ERROR, "Erreur d'initialisation de la SDL : %s", SDL_GetError());
        exit(-1);
    }
    zlog(stdout, INFO, "OK '%s'", "SDL is initialized.");

    /* création de la fenetre principale */
    (*game)->window = sdl_create_window("JEU DE LA TAUPE", (*game)->sw, (*game)->sh);
    if (!(*game)->window) exit(-1);
    zlog(stdout, INFO, "OK '%s'", "Window is initialized.");

    /* création du renderer */
    (*game)->renderer = sdl_create_renderer((*game)->window);
    if (!(*game)->renderer) exit(-1);
    zlog(stdout, INFO, "OK '%s'", "Renderer is initialized.");
    SDL_SetRenderDrawBlendMode((*game)->renderer, SDL_BLENDMODE_BLEND);

    /* init de ttf */
    sdl_init_text();
    (*game)->font = TTF_OpenFont("../data/fonts/Grooven-Shine.otf", 30);
    if (!(*game)->font) exit(-1);
    zlog(stdout, INFO, "OK '%s'", "Font is initialized.");

    /* ------ génération objets du jeu --------- */

    (*game)->state.keystate = SDL_GetKeyboardState(NULL);
    
    (*game)->game_rect.x = 0;
    (*game)->game_rect.y = 0;
    (*game)->game_rect.w = SCREEN_WIDTH;
    (*game)->game_rect.h = SCREEN_HEIGHT;

    (*game)->state.delay = GAME_DELAY;

    (*game)->state.nb_background = 3;
    (*game)->state.back
        = (struct background_s **) malloc(sizeof(struct background_s *) * (*game)->state.nb_background);
    
    (*game)->state.back[0]
        = animation_background_from_file((*game)->renderer, "../data/backgrounds/far.png");
    (*game)->state.back[1]
        = animation_background_from_file((*game)->renderer, "../data/backgrounds/sand.png");
    (*game)->state.back[2]
        = animation_background_from_file((*game)->renderer, "../data/backgrounds/foreground-merged.png");

    zlog(stdout, INFO, "OK '%s'", "Backgrounds are initialized");
    
    SDL_SetTextureAlphaMod((*game)->state.back[2]->t, 200);

    for (i = 0; i < (*game)->state.nb_background; ++i)
    {
        sdl_scale_rect_image(&(*game)->state.back[i]->r, (*game)->state.back[i]->t, (*game)->sh, (*game)->sw, 1);
    }

    zlog(stdout, INFO, "OK '%s'", "Backgrounds are scaled");

    (*game)->state.nb_entities = NB_GAME_ENTITIES+1;
    (*game)->state.entities = (struct entity_s **) malloc(sizeof(struct entity_s *) * (*game)->state.nb_entities);

    for (i = 0; i < n_sp; ++i) {
        e_fnames[i] = (char *) malloc(sizeof(char)*100);
    }

    tab[0] = 4;
    tab[1] = 4;
    tab[2] = 6;
    tab[3] = 2;
    tab[4] = 6;

    /* first entity */

    strncpy(e_fnames[0], "../data/sprites/fish/Idle.png", 99);
    strncpy(e_fnames[1], "../data/sprites/fish/Walk.png", 99);
    strncpy(e_fnames[2], "../data/sprites/fish/Attack.png", 99);
    strncpy(e_fnames[3], "../data/sprites/fish/Hurt.png", 99);
    strncpy(e_fnames[4], "../data/sprites/fish/Death.png", 99);
    
    (*game)->state.entities[0]
        = entity_create((*game)->renderer, e_fnames, n_sp,
                        tab, "fish", PLAYER, GOOD, 3);
    entity_scale((*game)->state.entities[0], 0.004*SCREEN_HEIGHT);

    /* second entities */

    strncpy(e_fnames[0], "../data/sprites/jellyfish/Idle.png", 99);
    strncpy(e_fnames[1], "../data/sprites/jellyfish/Walk.png", 99);
    strncpy(e_fnames[2], "../data/sprites/jellyfish/Attack.png", 99);
    strncpy(e_fnames[3], "../data/sprites/jellyfish/Hurt.png", 99);
    strncpy(e_fnames[4], "../data/sprites/jellyfish/Death.png", 99);

    for (i = 1; i < (*game)->state.nb_entities; ++i) {
        (*game)->state.entities[i]
            = entity_create((*game)->renderer, e_fnames, n_sp,
                            tab, "jellyfish", IA, BAD, 2);
        entity_scale((*game)->state.entities[i], 0.004*SCREEN_HEIGHT);
        (*game)->state.entities[i]->r.x = rand()%(SCREEN_WIDTH-(*game)->state.entities[i]->r.w);
        (*game)->state.entities[i]->r.y = rand()%(SCREEN_HEIGHT-(*game)->state.entities[i]->r.h);
    }
    
    
    zlog(stdout, INFO, "OK '%s'", "Entities are loaded");

    for (i = 0; i < n_sp; ++i) {
        free(e_fnames[i]);
    }
    free(e_fnames);
    free(tab);

    zlog(stdout, INFO, "OK '%s'", "tabs are free");

    return EXIT_SUCCESS;
}

/**
 * @brief game loop, initialise, manage events, update graphics, free
 *
 * @return exit code: 0 success, != 0 failure
 */
int game_loop(void)
{
    game_t * game = NULL;
    
    SDL_Event * event;
    
    game_initialisation(&game);

    event = &(game->state.event);
    
    /* Boucle de jeu */
    while (game->state.running == 1)
    {
        /* Boucle d'évènements */
        while (SDL_PollEvent(event))
        {
            switch(event->type)
            {
            case SDL_WINDOWEVENT:
        	switch (event->window.event)
        	{
        	case SDL_WINDOWEVENT_CLOSE:
        	    zlog(stdout, INFO, "sdl event window close", NULL);
        	    break;
        	}
        	break;
            case SDL_KEYDOWN:
                game_keyboard_state_update(&game->state);
        	break;
            case SDL_KEYUP:
                entity_change_state(game->state.entities[0], IDLE);
        	break;
            case SDL_MOUSEMOTION:
                /* update mouse position */
                SDL_GetMouseState(&game->state.mx, &game->state.my);
                break;
            case SDL_MOUSEBUTTONDOWN:
                game_mouse_state_update(&game->state);
        	break;
            case SDL_QUIT:
        	zlog(stdout, INFO, "event->type: SDL_QUIT", NULL);
        	game->state.running = 0;
                break;
            }
        }

        game_loop_state_update(&game->state);
        game_graphic_update(*game);

        SDL_RenderPresent(game->renderer);
        
        /* delai minimal = 1 */
        SDL_Delay(game->state.delay);
    }

    game_free_game(game);
    
    return EXIT_SUCCESS;
}
