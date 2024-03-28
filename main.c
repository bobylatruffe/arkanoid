#include "init.h"
#include "update.h"
#include "render.h"

Obj *createObj(double x, double y, double vx, double vy, double w, double h) {
    Obj *obj = calloc(1, sizeof(Obj));
    obj->x = x;
    obj->y = y;
    obj->vx = vx;
    obj->vy = vy;
    obj->w = w;
    obj->h = h;
}

int main() {
    GameContext *gameContext = init(700, 1200);

    Obj *ball = createObj(250, 100, 0, 900, 50, 50);
    gameContext->ball = ball;

    Obj *ship = createObj(250, 760, 1000, 0, 100, 10);
    gameContext->ship = ship;

    gameContext->isGameRunning = true;
    while (gameContext->isGameRunning) {
        if (gameContext->initRenderFinished == 0)
            update(gameContext);
        render(gameContext);
    }

    allFree(gameContext);
    return EXIT_SUCCESS;
}