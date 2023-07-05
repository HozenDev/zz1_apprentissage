#include "game.h"

const struct sprites_available_s sprites_available = 
{
    .fish = "fish",
    .jellyfish = "jellyfish"
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
        /* g_state->predators[0].e_sdl->is_in_animation = 0; */
        /* if (g_state->predators[0].e_sdl->state != ATTACK) */
        /*     entity_sdl_change_state(&g_state->predators[0].e_sdl, ATTACK); */
        break;
    case SDLK_DOWN:
        /* if (g_state->predators[0].e_sdl->state != WALK) */
        /*     entity_sdl_change_state(&g_state->predators[0].e_sdl, WALK); */
        break;
    case SDLK_UP:
        /* todo */
        break;
    case SDLK_LEFT:
        /* todo */
        break;
    case SDLK_RIGHT:
        /* todo */
        break;
    default:
        /* todo */
	break;
    }
}

/**
 * @brief updates game state on each loop turn
 * 
 * @param g_state, game state pointer
 */
void game_loop_state_update(game_state_t * g_state)
{
    int i, j,nb_compatible=0;
    
    simulation_get_perception(g_state->predators, g_state->prey);

    for(i = 0; i < g_state->nb_predator; i++)
    {
        /*init filtered_rules*/
        for(j=0;j<NB_RULES;j++) g_state->filtered_rules[j] = 0;
        /* filter rules */
        nb_compatible=simulation_filtrage_regle(g_state->predators[i], g_state->filtered_rules, g_state->brain);
        /* choisis une action */
        g_state->action[i] = simulation_choose_action(g_state->filtered_rules, g_state->brain,nb_compatible);
    }

    /* execute action */
    for(i = 0; i < g_state->nb_predator; i++)
        simulation_execute_action(&g_state->predators[i], g_state->action[i], g_state->predators, &g_state->prey);

    /* update sdl entity */
    for (i = 0; i < g_state->nb_predator; ++i) {
        g_state->predators[i].e_sdl->r.x = g_state->predators[i].x;
        g_state->predators[i].e_sdl->r.y = g_state->predators[i].y;
        if (g_state->predators[i].e_sdl->r.x > SCREEN_WIDTH-g_state->predators[i].e_sdl->r.w)
            g_state->predators[i].e_sdl->r.x = SCREEN_WIDTH-g_state->predators[i].e_sdl->r.w;
        if (g_state->predators[i].e_sdl->r.y > SCREEN_HEIGHT-g_state->predators[i].e_sdl->r.h)
            g_state->predators[i].e_sdl->r.y = SCREEN_HEIGHT-g_state->predators[i].e_sdl->r.h;
    }
    
    /* mise à jour parallax background */
    for (i = 0; i < g_state->nb_background; ++i)
        g_state->back[i]->r.x -= (i+1);
    
    /* update entity_sdl animations */
    entity_sdl_update_animation(g_state->prey.e_sdl, 0.1);
    for (i = 0; i < g_state->nb_predator; ++i)
        entity_sdl_update_animation(g_state->predators[i].e_sdl, 0.2);

    if (!g_state->end) /* if it is not the end of the game */
    {
        if (!g_state->prey.e_sdl->is_in_animation) entity_sdl_change_state(g_state->prey.e_sdl, IDLE);

        if (g_state->prey.pv <= 0)
        {
            g_state->end = 1;
        }
    }
    else { /* gestion fin de jeu */
        g_state->running = 0;
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
        /* clear game attributs */
        game->sw = 0;
        game->sh = 0;

        /* clear state */
        game->state.mx = 0;
        game->state.my = 0;

        if (game->state.prey.e_sdl)
        {
            entity_sdl_free(game->state.prey.e_sdl);
            game->state.prey.e_sdl = NULL;
        }
        
        for (i = 0; i < game->state.nb_predator; ++i)
        {
            entity_sdl_free(game->state.predators[i].e_sdl);
            game->state.predators[i].e_sdl = NULL;
        }

        for (i = 0; i < game->state.nb_background; ++i)
            animation_free_background(game->state.back[i]);
        free(game->state.back);

        if (game->font) TTF_CloseFont(game->font);

        /* free window and game renderer */
        if (game->renderer) {SDL_DestroyRenderer(game->renderer); game->renderer = NULL;}
        if (game->window) {SDL_DestroyWindow(game->window); game->window = NULL;}

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
    /* char buf[2048]; */
    int i;

    /* parallax background */
    for (i = 0; i < game.state.nb_background; ++i)
    {
        animation_render_background(game.renderer, game.state.back[i], game.sw, game.sh);
    }

    /* render prey */
    animation_render_sprite(game.renderer,
                            game.state.prey.e_sdl->sprites[game.state.prey.e_sdl->state],
                            game.state.prey.e_sdl->r);
    
    /* render predators */
    for (i = 0; i < game.state.nb_predator; ++i)
    {
        animation_render_sprite(game.renderer,
                                game.state.predators[i].e_sdl->sprites[game.state.predators[i].e_sdl->state],
                                game.state.predators[i].e_sdl->r);
    }
    
    /* sdl_render_image(game.renderer, game.back->t, game.back->r); */

    if (game.state.end) { /* écran de fin de jeu */

        /* screen of end */
        /* sdl_set_
           renderer_color(game.renderer, (SDL_Color) {.r=0, .g=0, .b=0, .a=150}); */
        /* sdl_draw_rect_coords(game.renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); */

        /* print a text */
        /* sdl_print_text(game.window, game.renderer, game.font, "Jouer", */
        /*                (SDL_Point) {.x = -1, .y = 200}, colors_available.WHITE); */
    }
    
    /* print score */
    /* sprintf(buf, "Score: %d", (int) game.state.score); */
    /* sdl_print_text(game.window, game.renderer, game.font, buf, */
    /*                (SDL_Point) {.x = 10, .y = 10}, colors_available.BLACK); */
}

/**
 * @brief Reset a game state
 *
 * @param g_state, game state pointer
 */
void game_state_reset(game_state_t * g_state)
{
    g_state->running = 1;
    g_state->end = 0;

    g_state->score = 0.0;
    g_state->delay = GAME_DELAY;
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

    /* ------ initialize game fields -------- */
    (*game)->sw = SCREEN_WIDTH;
    (*game)->sh = SCREEN_HEIGHT;
    (*game)->renderer = NULL;
    (*game)->window = NULL;
    (*game)->font = NULL;

    /* ------- initialize game state ------- */
    (*game)->state.mx = 0;
    (*game)->state.my = 0;
    (*game)->state.running = 1;
    (*game)->state.end = 0;
    (*game)->state.score = 0;
    (*game)->state.back = NULL;

    for (i = 0; i < NB_RULES; ++i) {
        (*game)->state.action[i] = 0;
        (*game)->state.filtered_rules[i] = 0;
    }

    /* ------ initialisation SDL2 --------- */
    
    /* SDL initialisation */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        zlog(stderr, ERROR, "Erreur d'initialisation de la SDL : %s", SDL_GetError());
        exit(-1);
    }
    zlog(stdout, INFO, "OK '%s'", "SDL is initialized.");

    /* création de la fenetre principale */
    (*game)->window = sdl_create_window("Shooter Marine", (*game)->sw, (*game)->sh);
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

    (*game)->state.game_rect.x = 10;
    (*game)->state.game_rect.y = 10;
    (*game)->state.game_rect.w = SCREEN_WIDTH-10;
    (*game)->state.game_rect.h = SCREEN_HEIGHT-10;

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

    for (i = 0; i < (*game)->state.nb_background; ++i)
    {
        sdl_scale_rect_image(&(*game)->state.back[i]->r, (*game)->state.back[i]->t, (*game)->sh, (*game)->sw, 1);
    }

    zlog(stdout, INFO, "OK '%s'", "Backgrounds are scaled");

    for (i = 0; i < n_sp; ++i) {
        e_fnames[i] = (char *) malloc(sizeof(char)*100);
    }

    tab[0] = 4;
    tab[1] = 4;
    tab[2] = 6;
    tab[3] = 2;
    tab[4] = 6;

    /* -------- initialize prey ----------- */

    strncpy(e_fnames[0], "../data/sprites/jellyfish/Idle.png", 99);
    strncpy(e_fnames[1], "../data/sprites/jellyfish/Walk.png", 99);
    strncpy(e_fnames[2], "../data/sprites/jellyfish/Attack.png", 99);
    strncpy(e_fnames[3], "../data/sprites/jellyfish/Hurt.png", 99);
    strncpy(e_fnames[4], "../data/sprites/jellyfish/Death.png", 99);

    entity_initialize_target(&(*game)->state.prey);
    (*game)->state.prey.e_sdl
        = entity_sdl_create((*game)->renderer, e_fnames, n_sp, tab, (float) PREY_SPEED);
    /* entity_sdl_scale((*game)->state.prey.e_sdl, 0.004*SCREEN_HEIGHT); */
    (*game)->state.prey.e_sdl->r.x = (*game)->state.prey.x;
    (*game)->state.prey.e_sdl->r.y = (*game)->state.prey.y;

    zlog(stdout, INFO, "OK '%s'", "Prey is loaded");

    /* -------- initialize predators ----------- */
    
    strncpy(e_fnames[0], "../data/sprites/fish/Idle.png", 99);
    strncpy(e_fnames[1], "../data/sprites/fish/Walk.png", 99);
    strncpy(e_fnames[2], "../data/sprites/fish/Attack.png", 99);
    strncpy(e_fnames[3], "../data/sprites/fish/Hurt.png", 99);
    strncpy(e_fnames[4], "../data/sprites/fish/Death.png", 99);

    (*game)->state.nb_predator = NB_PREDATOR;

    for (i = 0; i < (*game)->state.nb_predator; ++i)
    {
        entity_initialize(&(*game)->state.predators[i], SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
        (*game)->state.predators[i].e_sdl
            = entity_sdl_create((*game)->renderer, e_fnames, n_sp, tab, PREY_SPEED);
        /* entity_sdl_scale((*game)->state.predators[i].e_sdl, 0.004*SCREEN_HEIGHT); */
        (*game)->state.predators[i].e_sdl->r.x = 0;
        (*game)->state.predators[i].e_sdl->r.y = 0;
        entity_sdl_change_state((*game)->state.predators[i].e_sdl, WALK);
    }

    zlog(stdout, INFO, "OK '%s'", "Predators are loaded");
    
    /* ---------- free temp memory ------------ */

    for (i = 0; i < n_sp; ++i) {
        free(e_fnames[i]);
    }
    free(e_fnames);
    free(tab);

    zlog(stdout, INFO, "OK '%s'", "Initialisation of the game successed");

    return EXIT_SUCCESS;
}

/**
 * @brief game loop, initialise, manage events, update graphics, free
 *
 * @return exit code: 0 success, != 0 failure
 */
int game_loop(rules_t brain[NB_RULES], int * iter)
{
    game_t * game = NULL;
    
    SDL_Event * event;
    
    game_initialisation(&game);
    rules_copy_brain(brain, game->state.brain);

    event = &(game->state.event);
    
    /* Boucle de jeu */
    while (game->state.running == 1)
    {
        (*iter)++;
        
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
                /* game_keyboard_state_update(&game->state); */
        	break;
            case SDL_KEYUP:
                /* entity_sdl_change_state(game->state.predators[0].e_sdl, IDLE); */
        	break;
            case SDL_MOUSEMOTION:
                /* update mouse position */
                /* SDL_GetMouseState(&game->state.mx, &game->state.my); */
                break;
            case SDL_MOUSEBUTTONDOWN:
                /* game_mouse_state_update(&game->state); */
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
    sdl_quit_text();
    sdl_quit();
    
    return EXIT_SUCCESS;
}
