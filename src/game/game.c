#include "game.h"

void game_keyboard_state_update(game_state_t * g_state)
{
    int speed = 10;
    /* SDL_PumpEvents(); */
    
    if (!g_state->end) {
        if ( g_state->keystate[SDL_SCANCODE_RIGHT] && g_state->sprites[0]->d.x < SCREEN_WIDTH-g_state->sprites[0]->d.w)
            g_state->sprites[0]->d.x += speed;
        if ( g_state->keystate[SDL_SCANCODE_LEFT] && g_state->sprites[0]->d.x > 0)
            g_state->sprites[0]->d.x -= speed;
        if ( g_state->keystate[SDL_SCANCODE_UP] && g_state->sprites[0]->d.y > 0)
            g_state->sprites[0]->d.y -= speed;
        if ( g_state->keystate[SDL_SCANCODE_DOWN] && g_state->sprites[0]->d.y < SCREEN_HEIGHT-g_state->sprites[0]->d.h)
            g_state->sprites[0]->d.y += speed;
        if ( g_state->keystate[SDL_SCANCODE_SPACE] ) g_state->score += 1;
    }
    else {
        /* gestion fin de jeu */
    }
}

void game_loop_state_update(game_state_t * g_state)
{
    int i;
    /* animation_t * new_a = NULL; */
    
    for (i = 0; i < g_state->nb_background; ++i)
    {
        g_state->back[i]->r.x -= (i+1);
    }
    
    /* update sprites */
    for (i = 0; i < g_state->nb_sprite; ++i)
    {
        animation_update_sprite(&g_state->sprites[i], 0.2);
    }

    g_state->old_frame_time = g_state->new_frame_time;
    g_state->new_frame_time = SDL_GetTicks();
    if(g_state->new_frame_time > 0)
    {
        g_state->fps = 1 / ((float) (g_state->new_frame_time-g_state->old_frame_time)/1000);
    }

}

void game_change_sprites(game_t * game)
{
    (void) game;
    
    /* todo changer les sprites avec une probabilité */
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

        for (i = 0; i < game->state.nb_sprite; ++i)
        {
            animation_free_sprite(game->state.sprites[i]);
        }
        free(game->state.sprites);

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
    for (i = 0; i < game.state.nb_sprite; ++i)
    {
        animation_render_sprite(game.renderer, game.state.sprites[i]);
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

void game_state_reset(game_state_t * g_state)
{
    int i;
    animation_t * new_a = NULL;
    
    g_state->running = 1;
    g_state->end = 0;

    g_state->score = 0.0;
    g_state->delay = GAME_DELAY;
    
    /* reset all sprites */
    for (i = 0; i < g_state->nb_sprite; ++i)
    {
        new_a = animation_create_animation(g_state->sprites[i]->a->n, 1);
        g_state->sprites[i]->a->current_animation = 0.0;
        g_state->sprites[i]->d.x = 0;
        g_state->sprites[i]->d.y = 0;
        animation_change_animation(g_state->sprites[i], new_a);
        free(new_a);
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
    
    (*game) = (game_t *) malloc(sizeof(game_t));
    
    (*game)->sw = SCREEN_WIDTH;
    (*game)->sh = SCREEN_HEIGHT;

    /* initialize (*game) state */
    (*game)->state.mx = 0;
    (*game)->state.my = 0;
    (*game)->state.running = 1;
    (*game)->state.end = 0;

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
    zlog(stdout, INFO, "OK '%s'", "game_loop: Window is initialized.");

    /* création du renderer */
    (*game)->renderer = sdl_create_renderer((*game)->window);
    if (!(*game)->renderer) exit(-1);
    zlog(stdout, INFO, "OK '%s'", "game_loop: Renderer is initialized.");
    SDL_SetRenderDrawBlendMode((*game)->renderer, SDL_BLENDMODE_BLEND);

    /* init de ttf */
    sdl_init_text();
    (*game)->font = TTF_OpenFont("../data/fonts/Grooven-Shine.otf", 30);
    if (!(*game)->font) exit(-1);
    zlog(stdout, INFO, "OK '%s'", "game_loop: Font is initialized.");

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

    for (i = 0; i < (*game)->state.nb_background; ++i)
    {
        sdl_scale_rect_image(&(*game)->state.back[i]->r, (*game)->state.back[i]->t, (*game)->sh, (*game)->sw, 1);
    }

    (*game)->state.nb_sprite = NB_GAME_SPRITES;
    (*game)->state.sprites = (struct sprite_s **) malloc(sizeof(struct sprite_s *) * (*game)->state.nb_sprite);

    for (i = 0; i < (*game)->state.nb_sprite; ++i)
    {
        (*game)->state.sprites[i]
            = animation_spritesheet_from_file((*game)->renderer, "../data/sprites/fish/Idle.png", 4);
        animation_scale_sprite((*game)->state.sprites[i], 0.004*SCREEN_HEIGHT);
    }

    return EXIT_SUCCESS;
}

/**
 * @brief game loop, initialise, manage events, update graphics, free
 *
 * @return exit code: 0 success, != 0 failure
 */
int game_loop()
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
        	break;
            case SDL_KEYUP:
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

        game_keyboard_state_update(&game->state);
        game_loop_state_update(&game->state);
        game_graphic_update(*game);

        SDL_RenderPresent(game->renderer);
        
        /* delai minimal = 1 */
        SDL_Delay(game->state.delay);
    }

    game_free_game(game);
    
    return 0;
}
