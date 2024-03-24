#ifndef OBJECT_H
#define OBJECT_H

#include <SDL_image.h>
#include "type.h"


Background *createBackground(GameContext *gameContext, int x, int y, int w, int h, char *path);

void *createObj(GameContext *gameContext, int x, int y, int w, int h, int vx, int vy, char *path, TypeObj typeObj);

void createWireframeBall(Ball *ball);

void addBallInGameObjects(GameObjects *gameObjects, Ball *toAdd);

#endif