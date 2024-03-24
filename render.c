#include "render.h"

void render(GameContext *gameContext) {
    const int fps = 60;
    int delay_for_one_frame = 1000 / fps;

    Uint32 start_frame = 0;
    Uint32 end_frame = 0;
    Uint32 current_delay_frame = 0;
    static Uint32 delay_waiting_1_second = 0;

    start_frame = SDL_GetTicks();

    SDL_SetRenderDrawColor(gameContext->renderer, 0, 0, 0, 255);
    SDL_RenderClear(gameContext->renderer);

//    renderBackground(gameContext);
//    renderDecors(gameContext);
    renderBalls(gameContext);
    renderShips(gameContext);

    SDL_RenderPresent(gameContext->renderer);

    end_frame = SDL_GetTicks() - start_frame;

    if (end_frame < delay_for_one_frame) {
        SDL_Delay(delay_for_one_frame - end_frame);
    }
}

void renderBackground(GameContext *gameContext) {
    Background *background = gameContext->gameObjects->background;

    for (int x = 0; x <= gameContext->window_w; x += background->dest.w)
        for (int y = 0; y <= gameContext->window_h; y += background->dest.h) {
            background->dest.x = x;
            background->dest.y = y;
            SDL_RenderCopy(gameContext->renderer, background->texture, &background->src, &background->dest);
//#ifdef DEBUG
//            SDL_SetRenderDrawColor(gameContext->renderer, 255, 0, 0, 255);
//            SDL_RenderDrawPoint(gameContext->renderer, TLx(background), TLy(background));
//            SDL_RenderDrawPoint(gameContext->renderer, TRx(background), TRy(background));
//            SDL_RenderDrawPoint(gameContext->renderer, BLx(background), BLy(background));
//            SDL_RenderDrawPoint(gameContext->renderer, BRx(background), BRy(background));
//            SDL_RenderDrawPoint(gameContext->renderer, CENTx(background), CENTy(background));
//#endif
        }
}

void renderBalls(GameContext *gameContext) {
    Ball *balls = gameContext->gameObjects->balls;

    while (balls) {
        SDL_RenderCopy(gameContext->renderer, balls->texture, &balls->src, &balls->dest);

#ifdef DEBUG
        if (balls->wireframe) {
            for (int i = 0; i < balls->nbPointOfWireframe; i++) {
                SDL_RenderDrawPoint(gameContext->renderer, balls->wireframe[i].x, balls->wireframe[i].y);
            }
        }

        SDL_SetRenderDrawColor(gameContext->renderer, 255, 0, 0, 255);
        SDL_RenderDrawPoint(gameContext->renderer, TLx(balls), TLy(balls));
        SDL_RenderDrawPoint(gameContext->renderer, TRx(balls), TRy(balls));
        SDL_RenderDrawPoint(gameContext->renderer, BLx(balls), BLy(balls));
        SDL_RenderDrawPoint(gameContext->renderer, BRx(balls), BRy(balls));
        SDL_RenderDrawPoint(gameContext->renderer, CENTx(balls), CENTy(balls));
#endif
        balls = balls->next;
    }
}

void renderShips(GameContext *gameContext) {
    Ship *ship = gameContext->gameObjects->ships;

    SDL_RenderCopy(gameContext->renderer, ship->texture, &ship->src, &ship->dest);

#ifdef DEBUG
    SDL_SetRenderDrawColor(gameContext->renderer, 255, 0, 0, 255);
    SDL_RenderDrawPoint(gameContext->renderer, TLx(ship), TLy(ship));
    SDL_RenderDrawPoint(gameContext->renderer, TRx(ship), TRy(ship));
    SDL_RenderDrawPoint(gameContext->renderer, BLx(ship), BLy(ship));
    SDL_RenderDrawPoint(gameContext->renderer, BRx(ship), BRy(ship));
    SDL_RenderDrawPoint(gameContext->renderer, CENTx(ship), CENTy(ship));
#endif
}

void renderDecors(GameContext *gameContext) {

}