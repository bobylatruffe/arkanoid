//
// Created by bf on 24/03/24.
//

#ifndef ARKANOID_TYPE_H
#define ARKANOID_TYPE_H

#include <SDL.h>
#include <stdbool.h>

typedef struct {
    SDL_Rect wireframe;
    double x, y;
    double vx, vy;
    double w, h;
} Obj;

typedef struct {
    SDL_Window *sdlWindow;
    int w;
    int h;
} Window;

typedef struct {
    Window * window;
    SDL_Renderer *renderer;
    bool isGameRunning;
    double deltaTime;
    Obj *ball;
    Obj *ship;
} GameContext;

#endif //ARKANOID_TYPE_H
