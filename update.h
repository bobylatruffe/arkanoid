//
// Created by bf on 25/03/24.
//

#ifndef ARKANOID_UPDATE_H
#define ARKANOID_UPDATE_H

#include <math.h>
#include "type.h"

void screenToWorld(GameContext *gameContext, Obj *obj, int screenX, int screenY) {
    obj->x = screenX;
    obj->y = screenY;
}

void worldToScreen(GameContext *gameContext, Obj *obj) {
    obj->wireframe.x = (int) round(obj->x - obj->w / 2);
    obj->wireframe.y = (int) round(obj->y - obj->h / 2);
    obj->wireframe.w = (int) round(obj->w);
    obj->wireframe.h = (int) round(obj->h);

}

void update(GameContext *gameContext) {
    SDL_GetWindowSize(gameContext->window->sdlWindow, &gameContext->window->w, &gameContext->window->h);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            gameContext->isGameRunning = false;
        if (event.type == SDL_MOUSEMOTION) {
            screenToWorld(gameContext, gameContext->ball, event.motion.x, event.motion.y);
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
        }
    }

    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_RIGHT]) {
        gameContext->ship->x += gameContext->ship->vx;
    }
    if (keys[SDL_SCANCODE_LEFT]) {
        gameContext->ship->x -= gameContext->ship->vx;
    }
    if (keys[SDL_SCANCODE_SPACE]) {
        printf("angle = %lf\n",
               atan2(gameContext->ship->y - gameContext->ball->y,
                     gameContext->ship->x - gameContext->ball->x
               ) * 180 / M_PI
        );
    }

    Obj *ball = gameContext->ball;
    ball->x += ball->vx * gameContext->deltaTime;
    ball->y += ball->vy * gameContext->deltaTime;

    Obj *ship = gameContext->ship;

    worldToScreen(gameContext, ball);
    worldToScreen(gameContext, ship);
}

#endif //ARKANOID_UPDATE_H
