//
// Created by bf on 25/03/24.
//

#ifndef ARKANOID_UPDATE_H
#define ARKANOID_UPDATE_H

#include "type.h"

void worldToScreen(GameContext *gameContext, Obj *obj) {
    obj->wireframe.x = (int) round(
            (obj->x * (gameContext->window->w / 2.0)) + (gameContext->window->w / 2.0) - (obj->w / 2));
    obj->wireframe.y = (int) round(
            (-obj->y * (gameContext->window->h / 2.0)) + (gameContext->window->h / 2.0) - (obj->h / 2));
    obj->wireframe.w = (int) round(obj->w);
    obj->wireframe.h = (int) round(obj->h);
}

void update(GameContext *gameContext) {
    SDL_GetWindowSize(gameContext->window->sdlWindow, &gameContext->window->w, &gameContext->window->h);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            gameContext->isGameRunning = false;
    }

    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_RIGHT]) {
        gameContext->ship->x += gameContext->ship->vx;
    }
    if (keys[SDL_SCANCODE_LEFT]) {
        gameContext->ship->x -= gameContext->ship->vx;
    }

    Obj *ball = gameContext->ball;
    ball->x += ball->vx * gameContext->deltaTime;
    ball->y += ball->vy * gameContext->deltaTime;

    Obj *ship = gameContext->ship;
    if (ship->x <= -1.0)
        ship->x = -1.0;
    else if (ship->x >= 1)
        ship->x = 1.0;

    worldToScreen(gameContext, ball);
    worldToScreen(gameContext, ship);
}

#endif //ARKANOID_UPDATE_H
