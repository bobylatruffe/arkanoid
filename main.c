#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>


/* struct and typedef */

typedef struct Size {
    int w;
    int h;
} s_Size;

typedef struct game_object {
    SDL_Point coord;
    SDL_Point speed;
    SDL_Rect texture;
    s_Size size;
} s_Game_object;

typedef void (*Render)();

typedef struct to_renderer {
    Render to_rend;
    struct to_renderer *next;
} s_Renders;

/* global variable */

SDL_Renderer *g_pRenderer = NULL;

SDL_Texture *g_pBoard = NULL;
SDL_Rect rect_src_bg = {0};
SDL_Rect rect_dst_bg = {0};

const int FPS = 60;
const int DELAY = 1000 / FPS;
const int WINDOW_W = 500;
const int WINDOW_H = 500;

int nb_current_fps = 0;

Uint32 frame_start = 0,
        frame_end = 0,
        frame_delay = 0,
        frame_delay_for_counting_fps = 0;

void render_background() {
    for (int i = 0; i < WINDOW_W; i += 30) {
        for (int j = 0; j < WINDOW_H; j += 30) {
            SDL_RenderCopy(g_pRenderer, g_pBoard, &rect_src_bg, &rect_dst_bg);
            rect_dst_bg.x = i;
            rect_dst_bg.y = j;
        }
    }
}

void render_content(s_Renders *renders) {
    SDL_RenderClear(g_pRenderer);

    while (renders) {
        renders->to_rend();
        renders = renders->next;
    }

    SDL_RenderPresent(g_pRenderer);
}

void render(s_Renders *renders) {
    frame_start = SDL_GetTicks();

    render_content(renders);

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

void init_background() {
    g_pBoard = IMG_LoadTexture(g_pRenderer, "assets/arkanoid.png");
    if (g_pBoard) {
        printf("g_pBoard (background) : OK\n");
        rect_src_bg.x = 130;
        rect_src_bg.y = 385;
        rect_src_bg.w = 30;
        rect_src_bg.h = 30;

        rect_dst_bg.w = 30;
        rect_dst_bg.h = 30;
    } else
        printf("g_pBoard (background) : KO\n");
}

void init_renderer(SDL_Window *p_window) {
    g_pRenderer = SDL_CreateRenderer(p_window, -1, SDL_RENDERER_ACCELERATED);

    if (g_pRenderer) {
        printf("init_renderer (g_pRenderer) : OK\n");

        init_background();
    } else
        printf("init_renderer (g_pRenderer) : KO\n");
}

void init_window(SDL_Window *p_window) {
    p_window = SDL_CreateWindow("Arkanoid",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                WINDOW_W, WINDOW_H,
                                SDL_WINDOW_SHOWN);

    if (p_window) {
        printf("init_window (g_pWindow) : OK\n");

        init_renderer(p_window);
    } else
        printf("init_window (g_pWindow) : KO\n");
}

void init(SDL_Window *p_window) {
    if (!SDL_Init(SDL_INIT_EVERYTHING)) {
        printf("SDL_INIT_EVERTYTHING : OK\n");

        init_window(p_window);
    } else
        printf("SDL_INIT_EVERTYTHING : KO\n");
}

int main() {
    SDL_Window *p_window = NULL;
    init(p_window);

    s_Renders *renders = malloc(sizeof(s_Renders));
    renders->to_rend = render_background;
    renders->next = NULL;


    bool game_continu = true;
    SDL_Event event;
    while (game_continu) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                game_continu = false;
        }

        render(renders);
    }
}
