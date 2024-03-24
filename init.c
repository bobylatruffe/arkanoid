#include "init.h"
#include "type.h"

GameContext *init(int window_width, int window_height) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        printf("SDL_Init : KO\n");
        return NULL;
    }

    GameContext *gameContext = initWindowAndRenderer(window_width, window_height);
    SDL_GetWindowSize(gameContext->window, &gameContext->window_w, &gameContext->window_h);

    return gameContext;
}

GameContext *initWindowAndRenderer(int windowWith, int windowHeight) {
    GameContext *gameContext = calloc(1, sizeof(GameContext));

    if (!SDL_CreateWindowAndRenderer(windowWith, windowHeight, SDL_WINDOW_SHOWN,
                                     &gameContext->window, &gameContext->renderer))
        return gameContext;
    else
        return NULL;
}