#include "update.h"

void update(GameContext *gameContext) {
    SDL_GetWindowSize(gameContext->window, &gameContext->window_w, &gameContext->window_h);

    updateBalls(gameContext);
    updateShip(gameContext);
}

void updateBalls(GameContext *gameContext) {
    Ball * current_ball = gameContext->gameObjects->balls;

    while(current_ball) {
        current_ball->lastPos.x = current_ball->dest.x;
        current_ball->lastPos.y = current_ball->dest.y;

        current_ball->dest.x += SPEEDx(current_ball);
        current_ball->dest.y += SPEEDy(current_ball);

//        createWireframeBall(current_ball);

        checkBallCollisionShip(gameContext->gameObjects->balls, gameContext->gameObjects->ships);
        checkBallOutsideWindow(gameContext, current_ball);

        current_ball = current_ball->next;
    }
}

void updateShip(GameContext *gameContext) {
    Ship *ship = gameContext->gameObjects->ships;

    if(gameContext->isLeftDown)
        ship->dest.x -= SPEEDx(ship);
    if(gameContext->isRightDown)
        ship->dest.x += SPEEDx(ship);
}