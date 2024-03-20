#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>


/* struct and typedef */

typedef struct Size {
    int w;
    int h;
} s_size;

typedef struct game_object {
    SDL_Point coord;
    SDL_Point speed;
    SDL_Rect texture_src;
    SDL_Rect texture_dst;
    s_size size;
} s_game_obj;

typedef struct s_app_context {
    SDL_Window *p_window;
    SDL_Renderer *p_renderer;
    SDL_Texture *p_board;
    s_size window_size;
} s_app_context;

typedef void (*Render)();

typedef struct to_renderer {
    Render to_rend;

    struct to_renderer *next;
} s_renders;

SDL_Renderer *init_renderer(SDL_Window *p_window) {
    SDL_Renderer *p_renderer = SDL_CreateRenderer(p_window, -1, SDL_RENDERER_ACCELERATED);

    return p_renderer;
}

SDL_Window *init_window(s_size *size) {
    SDL_Window *p_window = SDL_CreateWindow("Arkanoid",
                                            SDL_WINDOWPOS_CENTERED,
                                            SDL_WINDOWPOS_CENTERED,
                                            size->w, size->h,
                                            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    return p_window;
}

bool init() {
    return !SDL_Init(SDL_INIT_EVERYTHING);
}

void handle_events(bool *game_continues) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            *game_continues = false;
    }
}

void render(s_app_context *app_ctx, s_game_obj *bg_obj) {
    static const int desired_fps = 60;
    static int real_fps = 0;
    static int delay_desired_per_frame = 1000 / desired_fps;
    static Uint32 time_start_frame = 0;
    static Uint32 delay_render_this_frame = 0;
    static Uint32 delay_total = 0;

    time_start_frame = SDL_GetTicks();
    // ici render le vrai contenu
    for(int i=0; i<app_ctx->window_size.w; i+=30)
        for(int j=0; j<app_ctx->window_size.h; j+=30) {
            bg_obj->texture_dst.x = i;
            bg_obj->texture_dst.y = j;

            SDL_RenderCopy(app_ctx->p_renderer, app_ctx->p_board, &bg_obj->texture_src, &bg_obj->texture_dst);
        }
    SDL_RenderPresent(app_ctx->p_renderer);

    delay_render_this_frame = SDL_GetTicks() - time_start_frame;

    if (delay_render_this_frame < delay_desired_per_frame) {
        SDL_Delay(delay_desired_per_frame - delay_render_this_frame);
    }

    delay_total += SDL_GetTicks() - time_start_frame;
    real_fps++;

    if (delay_total >= 1000) {
        printf("real_fps : %d\n", real_fps);
        real_fps = 0;
        delay_total = 0;
    }
}

void update(s_app_context *app_ctx) {
    SDL_GetWindowSize(app_ctx->p_window, &app_ctx->window_size.w, &app_ctx->window_size.h);
}

s_game_obj *init_background(SDL_Texture *sprite_board) {
    s_game_obj *bg_obj = calloc(1, sizeof(s_game_obj));
    bg_obj->texture_src.x = 130;
    bg_obj->texture_src.y = 385;
    bg_obj->texture_src.w = bg_obj->texture_dst.w = 30;
    bg_obj->texture_src.h = bg_obj->texture_dst.h = 30;

    return bg_obj;
}

int main() {
    if (!init()) {
        printf("sdl_init_everything : KO\n");
        return -1;
    }

    s_size window_size = {500, 500};
    SDL_Window *p_window = init_window(&window_size);
    if (!p_window) {
        printf("p_window : KO\n");
        return -1;
    }

    SDL_Renderer *p_renderer = init_renderer(p_window);
    if (!p_renderer) {
        printf("p_renderer : KO\n");
        return -1;
    }

    s_app_context *app_ctx = malloc(sizeof(s_app_context));
    app_ctx->p_window = p_window;
    app_ctx->p_renderer = p_renderer;

    SDL_Texture *board = IMG_LoadTexture(p_renderer, "./assets/arkanoid.png");
    if (!board) {
        printf("impossible de charger la texture sprite_board");
        return -1;
    }
    app_ctx->p_board = board;

    s_game_obj *bg_obj = init_background(board);

    bool game_continues = true;
    while (game_continues) {
        handle_events(&game_continues);
        update(app_ctx);
        render(app_ctx, bg_obj);
    }

    free(app_ctx);
    free(bg_obj);
}
