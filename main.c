#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>

SDL_Window *g_pWindow = NULL;
SDL_Renderer *g_pRenderer = NULL;

SDL_Texture *g_pBoard = NULL;
SDL_Rect rect_src_bg = {0};
SDL_Rect rect_dst_bg = {0};

int spaceship_w = 58;
int spaceship_h = 12;
SDL_Rect rect_src_spaceship = {0};
SDL_Rect rect_dst_spaceship = {0};

typedef int (*getX)();
typedef int (*getX)();

typedef struct ball {
    SDL_Point coord;
    SDL_Point speed;
    SDL_Rect texture;
} ball;

int size_ball_w = 14;
int size_ball_h = 14;
struct {
    int vx;
    int vy;
} speed_ball = {1, 1};
SDL_Rect rect_src_ball = {0};
SDL_Rect rect_dst_ball = {0};

int FPS = 1;
//const int DELAY = 1000 / FPS;
int DELAY;
const int WINDOW_W = 500;
const int WINDOW_H = 500;

int nb_current_fps = 0;

Uint32 frame_start = 0,
        frame_end = 0,
        frame_delay = 0,
        frame_delay_for_counting_fps = 0;

void ball_collision_spaceship() {
    int closestX, closestY;

    // Pour l'axe X
    if (rect_dst_ball.x + size_ball_w / 2 < rect_dst_spaceship.x) {
        closestX = rect_dst_spaceship.x;
    } else if (rect_dst_ball.x + size_ball_w / 2 > rect_dst_spaceship.x + spaceship_w) {
        closestX = rect_dst_spaceship.x + spaceship_w;
    } else {
        closestX = rect_dst_ball.x + size_ball_w / 2;
    }

    // Pour l'axe Y
    if (rect_dst_ball.y + size_ball_h / 2 < rect_dst_spaceship.y) {
        closestY = rect_dst_spaceship.y;
    } else if (rect_dst_ball.y + size_ball_h / 2 > rect_dst_spaceship.y + spaceship_h) {
        closestY = rect_dst_spaceship.y + spaceship_h;
    } else {
        closestY = rect_dst_ball.y + size_ball_h / 2;
    }

    // Calculer la distance entre le centre du cercle et ce point le plus proche
    int distanceX = rect_dst_ball.x + size_ball_w / 2 - closestX;
    int distanceY = rect_dst_ball.y + size_ball_h / 2 - closestY;
    int distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);

    // Si la distance est inférieure ou égale au carré du rayon du cercle, il y a collision
    if (distanceSquared <= (6 * 6)) {
        speed_ball.vx *= -1;
        speed_ball.vy *= -1;
    }

}

void move_ball() {
//    printf("ball(%d, %d)\n", rect_dst_ball.x, rect_dst_ball.y);

    rect_dst_ball.x += speed_ball.vx;
    rect_dst_ball.y += speed_ball.vy;

    if (rect_dst_ball.x + size_ball_w >= WINDOW_W || rect_dst_ball.x <= 0)
        speed_ball.vx *= -1;

    if (rect_dst_ball.y + size_ball_h >= WINDOW_H || rect_dst_ball.y <= 0)
        speed_ball.vy *= -1;
}

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
    SDL_RenderCopy(g_pRenderer, g_pBoard, &rect_src_ball, &rect_dst_ball);

    move_ball();

    ball_collision_spaceship();

    SDL_RenderPresent(g_pRenderer);
}

void render() {
    // TODO : Juste pour débug
    DELAY = 1000 / FPS;

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

void init_ball() {
    if (g_pBoard) {
        printf("g_pBoard (ball) : OK\n");

        rect_src_ball.x = 463;
        rect_src_ball.y = 142;
        rect_src_ball.w = 7;
        rect_src_ball.h = 7;

        rect_dst_ball.w = size_ball_w;
        rect_dst_ball.h = size_ball_h;
        rect_dst_ball.x = WINDOW_W / 2 - size_ball_w / 2;
        rect_dst_ball.y = WINDOW_H - 50;

    } else
        printf("g_pBoard (ball) : KO\n");
}

void init_spaceship() {
    if (g_pBoard) {
        printf("g_pBoard (spaceship): OK\n");

        rect_src_spaceship.x = 260;
        rect_src_spaceship.y = 143;
        rect_src_spaceship.w = rect_dst_spaceship.w = spaceship_w;;
        rect_src_spaceship.h = rect_dst_spaceship.h = spaceship_h;

        rect_dst_spaceship.x = WINDOW_W / 2 - spaceship_w / 2;
        rect_dst_spaceship.y = WINDOW_H - 24;
    } else
        printf("g_pBoard (spaceship) : KO\n");
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

void init_renderer() {
    g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, SDL_RENDERER_ACCELERATED);

    if (g_pRenderer) {
        printf("init_renderer (g_pRenderer) : OK\n");

        init_background();
        init_spaceship();
        init_ball();
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
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.scancode == SDL_SCANCODE_KP_1) {
                    FPS = 1;
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_KP_2) {
                    FPS = 20;
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_KP_3) {
                    FPS = 50;
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_KP_4) {
                    FPS = 100;
                }
            }
            if (event.type == SDL_MOUSEMOTION) {
                rect_dst_spaceship.x = event.motion.x - spaceship_w / 2;
            }
        }

        render();
    }
}
