#include "object.h"

Background *createBackground(GameContext *gameContext, int x, int y, int w, int h, char *path) {
    SDL_Texture *texture = IMG_LoadTexture(gameContext->renderer, path);
    if (!texture) {
        printf("createBackground() : Impossible de charger la texture\n");

        return NULL;
    }

    Background *background = calloc(1, sizeof(Background));
    background->texture = texture;
    background->src.x = x;
    background->src.y = y;
    background->src.w = w;
    background->src.h = h;

    return background;
}

#include <math.h>

void createWireframeBall(Ball *ball) {
    int rayon = ball->dest.w / 2;
    int estimationPoints = ceil(2 * M_PI * rayon);
    int nombreMaxPoints = estimationPoints + (int) ceil(estimationPoints * 0.1);

    if (ball->wireframe)
        free(ball->wireframe);

    ball->wireframe = calloc(nombreMaxPoints, sizeof(SDL_Point));

    int i = 0;
    for (int w = 0; w < rayon * 2; w++) {
        for (int h = 0; h < rayon * 2; h++) {
            int dx = w - rayon;
            int dy = h - rayon;
            int distance = dx * dx + dy * dy;
            if (distance < (rayon * rayon) && distance > ((rayon - 1) * (rayon - 1))) {
                ball->wireframe[i].x = CENTx(ball) + dx;
                ball->wireframe[i].y = CENTy(ball) + dy;
                i++;
            }
        }
    }

    ball->nbPointOfWireframe = i;
}

void *createObj(GameContext *gameContext, int x, int y, int w, int h, int vx, int vy, char *path, TypeObj typeObj) {
    SDL_Texture *texture = IMG_LoadTexture(gameContext->renderer, path);
    if (!texture) {
        printf("createObj() : Impossible de charger la texture\n");

        return NULL;
    }

    Ball *obj = calloc(1, sizeof(Ball));

    obj->texture = texture;
    obj->src.x = x;
    obj->src.y = y;
    obj->src.w = w;
    obj->src.h = h;
    obj->speed.x = vx;
    obj->speed.y = vy;
    obj->typeObj = typeObj;

    switch (typeObj) {
        case BALL:
            return obj;
        case SHIP:
            return (Ship *) obj;
    }

    return obj;
}

void addBallInGameObjects(GameObjects *gameObjects, Ball *toAdd) {
    if (gameObjects->balls == NULL) {
        gameObjects->balls = toAdd;
        return;
    }

    Ball *first = gameObjects->balls;
    while (first->next)
        first = first->next;

    first->next = toAdd;
}