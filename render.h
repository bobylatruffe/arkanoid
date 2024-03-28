//
// Created by bf on 24/03/24.
//

#ifndef ARKANOID_RENDER_H
#define ARKANOID_RENDER_H

#include <math.h>
#include "type.h"

void render_content(GameContext *ctx) {
    SDL_SetRenderDrawColor(ctx->renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(ctx->renderer, &ctx->ball->wireframe);

    SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(ctx->renderer, &ctx->ship->wireframe);

    SDL_SetRenderDrawColor(ctx->renderer, 0, 255, 255, 255);
//    SDL_RenderDrawLine(ctx->renderer,
//                       ctx->ball->x, ctx->ball->y,
//                       ctx->ball->x, ctx->ball->y + ctx->ball->vy * ctx->deltaTime);


    SDL_RenderPresent(ctx->renderer);
}

void render(GameContext *gameContext) {
    static const double FPS = 1.0;
    static const double DELAY_PER_FRAME = 1000.0f / FPS;
    static Uint32 cumul_seconds;
    static int current_fps = 0;

    Uint32 start_frame;
    Uint32 end_frame;

    start_frame = SDL_GetTicks();
// --- Le véritable rnedu ce trouve à partir d'ici
    if (gameContext->initRenderFinished == 0) {
        SDL_SetRenderDrawColor(gameContext->renderer, 0, 0, 0, 255);
        SDL_RenderClear(gameContext->renderer);
        render_content(gameContext);
    }
// --- jusqu'ici
    end_frame = SDL_GetTicks() - start_frame;

    if (end_frame < DELAY_PER_FRAME) {
        SDL_Delay((Uint32) DELAY_PER_FRAME - end_frame);
    }

    gameContext->deltaTime = (SDL_GetTicks() - start_frame) / 1000.0;

    current_fps++;
    cumul_seconds += SDL_GetTicks() - start_frame;
    if (cumul_seconds >= 1000) {
//        printf("current_fps : %d\n", current_fps);
        cumul_seconds = 0;
        current_fps = 0;
        if (gameContext->initRenderFinished != 0)
            gameContext->initRenderFinished--;
    }

}

#endif //ARKANOID_RENDER_H
