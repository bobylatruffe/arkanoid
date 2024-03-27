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
    obj->wireframe.x = (int) round(obj->x);
    obj->wireframe.y = (int) round(obj->y);
    obj->wireframe.w = (int) round(obj->w);
    obj->wireframe.h = (int) round(obj->h);
}

//void checkCollisionBallWithShip(GameContext *ctx, Obj *ball, Obj *ship) {
//    SDL_Point ballCenter = {(int) round(ball->x + ball->w / 2), (int) round(ball->y + ball->h)};
//
//    SDL_Point ballNextPos = {(int) round(ball->x + ball->w / 2),
//                             (int) round(ball->y + ball->vy * ctx->deltaTime)};
//
////    SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
//////    SDL_RenderClear(ctx->renderer);
////    SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 0, 255);
////    SDL_RenderDrawPoint(ctx->renderer, ballCenter.x, ballCenter.y);
////    SDL_RenderDrawPoint(ctx->renderer, ballNextPos.x, ballNextPos.y);
////
////    SDL_RenderDrawLine(ctx->renderer, ballCenter.x, ballCenter.y, ballNextPos.x, ballNextPos.y);
////    SDL_RenderPresent(ctx->renderer);
//
//    if (SDL_IntersectRectAndLine(&ctx->ship->wireframe, &ballCenter.x, &ballCenter.y, &ballNextPos.x, &ballNextPos.y)) {
//        printf("ok\n");
//        ball->vy *= -1;
//    }
//}



void checkCollisionBallWithWalls(const GameContext *gameContext, Obj *ball) {
    if (ball->y <= 0) {
        ball->y = 0;
        ball->vy *= -1;
    } else if (ball->x + ball->w >= gameContext->window->w) {
        ball->x = gameContext->window->w - ball->w;
        ball->vx *= -1;
    } else if (ball->x <= 0) {
        ball->x = 0;
        ball->vx *= -1;
    } else if (ball->y >= gameContext->window->h) {
        ball->y = 0;
    }
}

void eventHandler(GameContext *gameContext) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            gameContext->isGameRunning = false;
        if (event.type == SDL_MOUSEMOTION) {
//            screenToWorld(gameContext, gameContext->ball, event.motion.x, event.motion.y);
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
        }
    }

    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_RIGHT]) {
        gameContext->ship->x += gameContext->ship->vx * gameContext->deltaTime;
    }
    if (keys[SDL_SCANCODE_LEFT]) {
        gameContext->ship->x -= gameContext->ship->vx * gameContext->deltaTime;
    }
    if (keys[SDL_SCANCODE_SPACE]) {
        printf("angle = %lf\n",
               atan2(gameContext->ship->y - gameContext->ball->y,
                     gameContext->ship->x - gameContext->ball->x
               ) * 180 / M_PI
        );
    }
}

void update(GameContext *gameContext) {
    SDL_GetWindowSize(gameContext->window->sdlWindow, &gameContext->window->w, &gameContext->window->h);

    eventHandler(gameContext);

    gameContext->ball->x += gameContext->ball->vx * gameContext->deltaTime;
    gameContext->ball->y += gameContext->ball->vy * gameContext->deltaTime;

    checkCollisionBallWithShip(gameContext, gameContext->ball, gameContext->ship);
    checkCollisionBallWithWalls(gameContext, gameContext->ball);

    worldToScreen(gameContext, gameContext->ball);
    worldToScreen(gameContext, gameContext->ship);
}

#endif //ARKANOID_UPDATE_H
