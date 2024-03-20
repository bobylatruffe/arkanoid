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
    SDL_Rect texture;
    s_size size;
} s_gmae_obj;

typedef struct app_context {
    SDL_Window * p_window;
    SDL_Renderer  * p_renderer;
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
                                            SDL_WINDOW_SHOWN);

    return p_window;
}

bool init() {
    return !SDL_Init(SDL_INIT_EVERYTHING);
}

void render() {
    static int desired_fps = 60;
    static int real_fps = 0;
    static int delay_desired_per_frame = 1000 / 60;
    static Uint32 time_start_frame = 0;
    static Uint32 delay_render_this_frame = 0;
    static Uint32 delay_total = 0;

    time_start_frame = SDL_GetTicks();
    // ici render le vrai contenu
    delay_render_this_frame = SDL_GetTicks() - time_start_frame;

    if(delay_render_this_frame < delay_desired_per_frame){
        SDL_Delay(delay_desired_per_frame - delay_render_this_frame);
    }

    delay_total += SDL_GetTicks() - time_start_frame;
    real_fps++;

    if(delay_total >= 1000) {
        printf("real_fps : %d\n", real_fps);
        real_fps = 0;
        delay_total = 0;
    }
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

    s_app_context * app_ctx = malloc(sizeof(s_app_context));
    app_ctx->p_window = p_window;
    app_ctx->p_renderer = p_renderer;

    bool game_continues = true;
    while(game_continues) {
//        handle_events();
//        update();
        render();
    }

    free(app_ctx);
}
