#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>

SDL_Window *g_pWindow = NULL;
SDL_Renderer *g_pRenderer = NULL;

SDL_Texture *g_pBoard = NULL;
SDL_Rect rect_src_bg = {0};
SDL_Rect rect_dst_bg = {0};

SDL_Rect rect_src_spaceship = {0};
SDL_Rect rect_dst_spaceship = {0};

const int FPS = 60;
const int DELAY = 1000 / FPS;
const int WINDOW_W = 600;
const int WINDOW_H = 800;

int nb_current_fps = 0;

Uint32 frame_start = 0, frame_end = 0, frame_delay = 0, frame_delay_for_counting_fps = 0;


void render_contents() {
    SDL_RenderClear(g_pRenderer);

    for (int i = 0; i < WINDOW_W; i += 30) {
        for (int j = 0; j < WINDOW_H; j += 30) {
            SDL_RenderCopy(g_pRenderer, g_pBoard, &rect_src_bg, &rect_dst_bg);
            rect_dst_bg.x = i;
            rect_dst_bg.y = j;
        }
    }

    SDL_RenderCopy(g_pRenderer, g_pBoard, &rect_src_spaceship, &rect_dst_spaceship);

    SDL_RenderPresent(g_pRenderer);
}

void render() {
    frame_start = SDL_GetTicks();

    render_contents();

    frame_end = SDL_GetTicks();
    frame_delay = frame_end - frame_start;
    if (frame_delay < DELAY) {
        SDL_Delay(DELAY - frame_delay);
    }

    frame_delay_for_counting_fps += SDL_GetTicks() - frame_start;
    nb_current_fps++;

    if (frame_delay_for_counting_fps >= 1000) {
        printf("nb fps : %d\n", nb_current_fps);
        nb_current_fps = 0;
        frame_delay_for_counting_fps = 0;
    }
}

void init_spaceship() {
    if(g_pBoard) {
        printf("g_pBoard : OK\n");

        rect_src_spaceship.x = 260;
        rect_src_spaceship.y = 143;
        rect_src_spaceship.w = rect_dst_spaceship.w = 58;
        rect_src_spaceship.h = rect_dst_spaceship.h = 12;

        rect_dst_spaceship.x = WINDOW_W / 2 - 58 / 2;
        rect_dst_spaceship.y = WINDOW_H - 24;

    }
    else
        printf("g_pBoard : KO\n");
}

void init_background() {
    g_pBoard = IMG_LoadTexture(g_pRenderer, "assets/arkanoid.png");
    if (g_pBoard) {
        printf("g_pBoard : OK\n");
        rect_src_bg.x = 130;
        rect_src_bg.y = 385;
        rect_src_bg.w = 30;
        rect_src_bg.h = 30;

        rect_dst_bg.w = 30;
        rect_dst_bg.h = 30;
    } else
        printf("g_pBoard : KO\n");
}

void init_renderer() {
    g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, SDL_RENDERER_ACCELERATED);

    if (g_pRenderer) {
        printf("init_renderer (g_pRenderer) : OK\n");

        init_background();
        init_spaceship();
    } else
        printf("init_renderer (g_pRenderer) : KO\n");
}

void init_window() {
    g_pWindow = SDL_CreateWindow("Arkanoid",
                                 SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED,
                                 WINDOW_W, WINDOW_H,
                                 SDL_WINDOW_SHOWN);

    if (g_pWindow) {
        printf("init_window (g_pWindow) : OK\n");

        init_renderer();
    } else
        printf("init_window (g_pWindow) : KO\n");
}

void init() {
    if (!SDL_Init(SDL_INIT_EVERYTHING)) {
        printf("SDL_INIT_EVERTYTHING : OK\n");

        init_window();
    } else
        printf("SDL_INIT_EVERTYTHING : KO\n");
}


int main() {
    init();

    bool game_continu = true;
    SDL_Event event;
    while (game_continu) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                game_continu = false;
        }

        render();
    }
}
