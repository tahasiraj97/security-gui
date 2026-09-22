#include <SDL3/SDL.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} AppState;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
    static AppState state;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(
        "Security Dashboard",
        1280,
        720,
        0,
        &state.window,
        &state.renderer
    )) {
        SDL_Log("Window creation failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    *appstate = &state;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *state = appstate;

    SDL_SetRenderDrawColor(state->renderer, 80, 20, 20, 255);
    SDL_RenderClear(state->renderer);

    SDL_FRect box = {
        .x = 100,
        .y = 100,
        .w = 300,
        .h = 150
    };

    SDL_SetRenderDrawColor(state->renderer, 30, 200, 100, 255);
    SDL_RenderFillRect(state->renderer, &box);

    SDL_RenderPresent(state->renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    AppState *state = appstate;

    if (state != NULL) {
        SDL_DestroyRenderer(state->renderer);
        SDL_DestroyWindow(state->window);
    }
}