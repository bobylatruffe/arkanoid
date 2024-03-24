#ifndef TYPE_H
#define TYPE_H

#include <SDL.h>

#define TLx(gameObj) (gameObj->dest.x)
#define TLy(gameObj) (gameObj->dest.y)

#define TRx(gameObj) (TLx(gameObj) + gameObj->dest.w)
#define TRy(gameObj) (TLy(gameObj))

#define BLx(gameObj) (TLx(gameObj))
#define BLy(gameObj) (TLy(gameObj) + gameObj->dest.h)

#define BRx(gameObj) (TRx(gameObj))
#define BRy(gameObj) (BLy(gameObj))

#define CENTx(gameObj) (TRx(gameObj) - gameObj->dest.w / 2)
#define CENTy(gameObj) (BLy(gameObj) - gameObj->dest.h / 2)

#define SPEEDx(gameObj) (gameObj->speed.x)
#define SPEEDy(gameObj) (gameObj->speed.y)

typedef struct Background {
    SDL_Texture *texture;
    SDL_Rect src;
    SDL_Rect dest;
    struct Background *next;
} Background;

typedef enum TypeObj {
    BALL,
    SHIP
} TypeObj;

typedef struct obj {
    TypeObj typeObj;
    SDL_Texture *texture;
    SDL_Rect src;
    SDL_Rect dest;
    SDL_Point speed;
    SDL_Point lastPos;
    SDL_Point *wireframe; // TODO : A supp après demande prof
    int nbPointOfWireframe; // TODO : A supp après demande prof
    struct obj *next;
} Ball, Ship;

//typedef struct Ship {
//    SDL_Texture *texture;
//    SDL_Rect src;
//    SDL_Rect dest;
//    SDL_Point speed;
//    struct Ship *next;
//} Ship

typedef struct GameObjects {
    Background *background;

    Ball *balls;
    Ship *ships;
} GameObjects;

typedef struct GameContext {
    SDL_Renderer *renderer;
    SDL_Window *window;
    int window_w;
    int window_h;
    int isLeftDown;
    int isRightDown;
    GameObjects *gameObjects;
} GameContext;

#endif