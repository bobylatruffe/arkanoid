#ifndef CHECK_H
#define CHECK_H

#include <stdbool.h>
#include "type.h"

void checkBallOutsideWindow(GameContext *gameContext, Ball *ball);

void checkBallCollisionOtherBalls(Ball *ball, Ball *other);

void checkBallCollisionShip(Ball *balls, Ship *ship);

#endif
