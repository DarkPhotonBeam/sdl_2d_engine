#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <2DEngine/game.h>

#define WINDOW_FLAGS (0) // SDL_WINDOW_FULLSCREEN

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    Game_AppState *app_state = Game_AppState_Create();

    app_state->cam->zoom = 2;

    /* Create the window */
    if (!SDL_CreateWindowAndRenderer("2D Engine Demo", 1280, 720, WINDOW_FLAGS, &app_state->window, &app_state->renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    int w;
    int h;
    SDL_GetWindowSize(app_state->window, &w, &h);
    SDL_Log("GetWindowSize -> %d, %d", w, h);

    Game_Object *obj1 = Game_Object_Create();
    obj1->rbody->pos.x = (double)(w>>1);
    obj1->rbody->pos.y = (double)(h>>1);
    obj1->rbody->mass = 1000;
    obj1->rbody->has_gravity = true;

    const SDL_Color blue = {.r=0,.g=0,.b=255,.a=255};

    Game_Object *obj2 = Game_Object_Create();
    obj2->rbody->pos.x = (double)((w>>1) + 32);
    obj2->rbody->pos.y = (double)(h>>1);
    obj2->rbody->vel.y = 0.00005;
    obj2->rbody->has_gravity = false;
    obj2->material->color = blue;

    Game_AddObject(app_state, obj1);
    Game_AddObject(app_state, obj2);

    *appstate = app_state;
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    Game_AppState *game = appstate;
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.key) {
            case SDLK_ESCAPE:
            case SDLK_Q:
                SDL_Log("QUIT");
                return SDL_APP_SUCCESS;
            case SDLK_A:
            case SDLK_LEFT:
                game->key_state->left = 1;
                game->cam->vel.x = -1;
                break;
            case SDLK_D:
            case SDLK_RIGHT:
                game->key_state->right = 1;
                game->cam->vel.x = 1;
                break;
            case SDLK_S:
            case SDLK_DOWN:
                game->key_state->down = 1;
                game->cam->vel.y = 1;
                break;
            case SDLK_W:
            case SDLK_UP:
                game->key_state->up = 1;
                game->cam->vel.y = -1;
                break;
            default:
                SDL_Log("Unhandled Key Down");
                break;
        }
    } else if (event->type == SDL_EVENT_KEY_UP) {
        switch (event->key.key) {
            case SDLK_A:
            case SDLK_LEFT:
                game->key_state->left = 0;
                if (!game->key_state->right) game->cam->vel.x = 0;
                break;
            case SDLK_D:
            case SDLK_RIGHT:
                game->key_state->right = 0;
                if (!game->key_state->left) game->cam->vel.x = 0;
                break;
            case SDLK_S:
            case SDLK_DOWN:
                game->key_state->down = 0;
                if (!game->key_state->up) game->cam->vel.y = 0;
                break;
            case SDLK_W:
            case SDLK_UP:
                game->key_state->up = 0;
                if (!game->key_state->down) game->cam->vel.y = 0;
                break;
            default:
                SDL_Log("Unhandled Key Up");
                break;
        }
    } else if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    } else if (event->type == SDL_EVENT_MOUSE_WHEEL) {
        constexpr double min_zoom = 0.001;
        const double amt = event->wheel.y * game->cam->zoom * 0.5;
        game->cam->zoom += amt;
        if (game->cam->zoom < min_zoom) game->cam->zoom = min_zoom;
    }
    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    const Game_AppState *state = appstate;

    Game_Compute(appstate);
    Game_Render(appstate, 1);

    SDL_RenderPresent(state->renderer);

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    Game_AppState_Destroy(appstate);
}

