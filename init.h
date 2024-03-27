//
// Created by bf on 24/03/24.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "DanglingPointer"
#ifndef ARKANOID_INIT_H
#define ARKANOID_INIT_H

#include "type.h"

void allFree(GameContext *gameContext) {
    if (gameContext->window)
        SDL_DestroyWindow(gameContext->window->sdlWindow);

    if (gameContext->renderer)
        SDL_DestroyRenderer(gameContext->renderer);

    SDL_Quit();
    free(gameContext);
}

void infoAndExit(GameContext *gameContext, char *nameFunc) {
    printf("Error in %s : %s\n", nameFunc, SDL_GetError());
    allFree(gameContext);
    exit(EXIT_FAILURE);
}

GameContext *init(int w, int h) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        infoAndExit(NULL, "SDL_Init()");
    }

    GameContext *gameContext = calloc(1, sizeof(GameContext));

    SDL_Window *sdlWindow = SDL_CreateWindow("Arkanoid", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                             w, h, SDL_WINDOW_SHOWN);
    if (!sdlWindow)
        infoAndExit(gameContext, "SDL_CreateWindow");

    Window *window = calloc(1, sizeof(Window));
    window->sdlWindow = sdlWindow;
    gameContext->window = window;
    SDL_GetWindowSize(gameContext->window->sdlWindow, &gameContext->window->w, &gameContext->window->h);

    gameContext->renderer = SDL_CreateRenderer(gameContext->window->sdlWindow, -1, SDL_RENDERER_ACCELERATED);

    if (!gameContext->renderer)
        infoAndExit(gameContext, "SDL_CreateRenderer");

    return gameContext;
}

#endif //ARKANOID_INIT_H

#pragma clang diagnostic pop