#include "check.h"

void checkBallOutsideWindow(GameContext *gameContext, Ball *ball) {
    ball->dest.x += ball->speed.x;
    ball->dest.y += ball->speed.y;


    if (TRx(ball) >= gameContext->window_w) {
        ball->dest.y -= ball->speed.y;
        ball->speed.x *= -1;
        ball->dest.x = gameContext->window_w - ball->dest.w - 1;
        return;
    }

    if (TLx(ball) <= 0) {
        ball->dest.y -= ball->speed.y;
        ball->speed.x *= -1;
        ball->dest.x = 1;
        return;
    }

    if (TLy(ball) <= 0) {
        ball->dest.x -= ball->speed.x;
        ball->speed.y *= -1;
        ball->dest.y = 1;
        return;
    }

//    if (BLy(ball) >= gameContext->window_h) {
//        ball->dest.x -= ball->speed.x;
//        ball->speed.y *= -1;
//        ball->dest.y = gameContext->window_h - ball->dest.w;
//        return;
//    }

    if (TLy(ball) > gameContext->window_h + ball->dest.h) {
        printf("Balle dehors par le bas\n");
        exit(0);
    }

    ball->dest.x -= ball->speed.x;
    ball->dest.y -= ball->speed.y;

//    SDL_Point *wireframe = ball->wireframe;
//    for (int i = 0; i < ball->nbPointOfWireframe; i++) {
//        if (wireframe[i].x >= gameContext->window_w || wireframe[i].x <= 0) {
//            ball->speed.x *= -1;
//            return;
//        }
//        if (wireframe[i].y >= gameContext->window_h || wireframe[i].y <= 0) {
//            ball->speed.y *= -1;
//            return;
//        }
//    }
}

// TODO : A continuer plus tard
void checkBallCollisionOtherBalls(Ball *ball, Ball *other) {
    if (ball != other) {
        if (TLx(ball) <= TRx(other)) {
            printf("%d %d\n", TRx(ball), TLx(other));
            ball->speed.x *= -1;
            other->speed.x *= -1;
        }
    }

    other = other->next;
}

void checkBallCollisionShip(Ball *balls, Ship *ship) {
//    printf("PREMIER\n");
    while (balls) {
//        if (balls->speed.y > 1) {
//            int ballBeforeMovingY = balls->dest.y - balls->speed.y;
//            balls->dest.y -= balls->speed.y;
//
//            for (int i = 1; i <= balls->speed.y; i++) {
//                balls->dest.y++;
//                if (BRy(balls) >= TLy(ship)) {
//                    if (BRy(balls) > CENTy(ship))
//                        return;
//
//                    balls->speed.y *= -1;
//                    return;
//                }
//            }
//        }

//        balls->dest.y = originY;
//
//        balls->dest.x += balls->speed.x;
//        balls->dest.y += balls->speed.y;
//
//        if (SDL_HasIntersection(&balls->dest, &ship->dest)) {
//            if (BRy(balls) > CENTy(ship))
//                return;
//
////            printf("mdr\n");
//            balls->speed.y *= -1;
//            balls->dest.y = ship->dest.y - balls->dest.h;
//        }
//
//        balls->dest.x -= balls->speed.x;
//        balls->dest.y -= balls->speed.y;

        balls = balls->next;
    }
}