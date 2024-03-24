#include "init.h"
#include "render.h"
#include "object.h"
#include "update.h"

bool eventHandler(SDL_Event *event, GameContext *gameContext) {
    while (SDL_PollEvent(event)) {
        if ((*event).type == SDL_QUIT)
            return false;
    }

    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_LEFT]) {
        gameContext->isLeftDown = 1;
    } else {
        gameContext->isLeftDown = 0;
    }
    if (keys[SDL_SCANCODE_RIGHT])
        gameContext->isRightDown = 1;
    else
        gameContext->isRightDown = 0;

    if (keys[SDL_SCANCODE_SPACE])
        SDL_Delay(1000);

    return true;
}

int main() {
    GameContext *gameContext = init(700, 800);
    GameObjects *gameObjects = calloc(1, sizeof(GameObjects));
    gameContext->gameObjects = gameObjects;

    Background *background = createBackground(gameContext, 129, 386, 30, 30, "./assets/arkanoid.png");
    background->dest.w = background->dest.h = 30;
    gameObjects->background = background;


    for (int i = 1; i < 2; i++) {
        Ball *ball = createObj(gameContext, 0, 0, 22, 22, 0, 10, "./assets/balls.png", BALL);
//        ball->dest.y = gameContext->window_h - 52;
//        ball->dest.x = gameContext->window_w / 2 - 11;
        ball->dest.y = 250;
        ball->dest.x = 350;
        ball->dest.w = ball->dest.h = 18;
//        createWireframeBall(ball);
        addBallInGameObjects(gameObjects, ball);
    }

    Ship *ship = createObj(gameContext, 235, 175, 44, 11, 10, 10, "./assets/arkanoid.png", SHIP);
    ship->dest.y = gameContext->window_h - 30;
    ship->dest.x = gameContext->window_w / 2 - 22;
    ship->dest.w = 74;
    ship->dest.h = 15;
    gameObjects->ships = ship;

    SDL_Event event;
    while (eventHandler(&event, gameContext)) {
        update(gameContext);
        render(gameContext);
    }

    return 0;
}
