#ifndef INIT_H
#define INIT_H

#include <stdbool.h>
#include "type.h"

GameContext *init(int window_width, int window_height);

GameContext *initWindowAndRenderer(int windowWith, int windowHeight);

#endif