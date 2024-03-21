#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>


/* struct and typedef */

typedef struct Size {
    int w;
    int h;
} s_size;

typedef struct game_object {
    SDL_Rect tx_src;
    SDL_Rect tx_dst;
    int sx, sy; // speed
} s_game_obj;

typedef struct s_app_context {
    SDL_Window *p_window;
    SDL_Renderer *p_renderer;
    SDL_Texture *p_board;
    s_size window_size;
    SDL_Point mouse;
    int off_set_x;
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

void check_collision_ball_spaceship(s_game_obj *ball, s_game_obj *spaceship) {
    int closestX, closestY;

    // Pour l'axe X
    if (ball->tx_dst.x + ball->tx_dst.w / 2 < spaceship->tx_dst.x) {
        closestX = spaceship->tx_dst.x;
    } else if (ball->tx_dst.x + ball->tx_dst.w / 2 > spaceship->tx_dst.x + spaceship->tx_dst.w) {
        closestX = spaceship->tx_dst.x + spaceship->tx_dst.w;
    } else {
        closestX = ball->tx_dst.x + ball->tx_dst.w / 2;
    }

    // Pour l'axe Y
    if (ball->tx_dst.y + ball->tx_dst.w / 2 < spaceship->tx_dst.y) {
        closestY = spaceship->tx_dst.y;
    } else if (ball->tx_dst.y + ball->tx_dst.w / 2 > spaceship->tx_dst.y + spaceship->tx_dst.h) {
        closestY = spaceship->tx_dst.y + spaceship->tx_dst.h;
    } else {
        closestY = ball->tx_dst.y + ball->tx_dst.w / 2;
    }

    // Calculer la distance entre le centre du cercle et ce point le plus proche
    int distanceX = ball->tx_dst.x + ball->tx_dst.w / 2 - closestX;
    int distanceY = ball->tx_dst.y + ball->tx_dst.w / 2 - closestY;
    int distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);

    // Si la distance est inférieure ou égale au carré du rayon du cercle, il y a collision
    if (distanceSquared <= (6 * 6)) {
        ball->sx *= -1;
        ball->sy *= -1;
    }
}

void handle_events(s_app_context *app_ctxt, bool *game_continues) {
    // TODO : Est-ce réllement de créer cette variable à chaque boucle ?
    // Mais pas de variable globale ! Peut-être dans le contexte de l'application ?...
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            *game_continues = false;

        if (event.type == SDL_MOUSEMOTION) {
            app_ctxt->mouse.x = event.motion.x;
        }
    }

    // TODO : Est-ce réllement de créer cette variable à chaque boucle ?
    // Mais pas de variable globale ! Peut-être dans le contexte de l'application ?...
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_LEFT]) {
        app_ctxt->off_set_x -= 1;
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
        app_ctxt->off_set_x += 1;
    }
}

void render(s_app_context *app_ctx,
            s_game_obj *bg_obj,
            s_game_obj *ball_obj,
            s_game_obj *spaceship) {
    static const int desired_fps = 60;
    static int real_fps = 0;
    static int delay_desired_per_frame = 1000 / desired_fps;
    static Uint32 time_start_frame = 0;
    static Uint32 delay_render_this_frame = 0;
    static Uint32 delay_total = 0;

    // ---------------
    time_start_frame = SDL_GetTicks();

    for (int i = 0; i < app_ctx->window_size.w; i += 30)
        for (int j = 0; j < app_ctx->window_size.h; j += 30) {
            bg_obj->tx_dst.x = i;
            bg_obj->tx_dst.y = j;

            SDL_RenderCopy(app_ctx->p_renderer, app_ctx->p_board, &bg_obj->tx_src, &bg_obj->tx_dst);
        }

    SDL_RenderCopy(app_ctx->p_renderer, app_ctx->p_board, &ball_obj->tx_src, &ball_obj->tx_dst);
    SDL_RenderCopy(app_ctx->p_renderer, app_ctx->p_board, &spaceship->tx_src, &spaceship->tx_dst);

    SDL_RenderPresent(app_ctx->p_renderer);

    delay_render_this_frame = SDL_GetTicks() - time_start_frame;
    // -----------------

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

void update(s_app_context *app_ctx,
            s_game_obj *ball_obj,
            s_game_obj *spaceship) {
    SDL_GetWindowSize(app_ctx->p_window, &app_ctx->window_size.w, &app_ctx->window_size.h);

// GESTION VAISSEAU
// TODO : Prévoir que peut-être un appui long sur une touche augmente de manière croissante la vitesse de déplacement
// alors qu'un seul appuis ne déplace que d'un pixel.
    spaceship->tx_dst.x = app_ctx->window_size.w / 2 - spaceship->tx_dst.w / 2;
    spaceship->tx_dst.x += app_ctx->off_set_x * spaceship->sx;

// Pour contrôler avec la souris
//    spaceship->tx_dst.x = app_ctx->mouse.x - spaceship->tx_dst.w / 2;
//    spaceship->tx_dst.y = app_ctx->window_size.h - 11;

// GESTION BALLE
    ball_obj->tx_dst.x += ball_obj->sx;
    ball_obj->tx_dst.y += ball_obj->sy;

    check_collision_ball_spaceship(ball_obj, spaceship);

    if (ball_obj->tx_dst.y + ball_obj->tx_dst.h >= app_ctx->window_size.h) {
        ball_obj->tx_dst.x = 12;
        ball_obj->tx_dst.y = 12;
    }
    if (ball_obj->tx_dst.x + ball_obj->tx_dst.w >= app_ctx->window_size.w ||
        ball_obj->tx_dst.x <= 0) {
        ball_obj->sx *= -1;
    }
    if (ball_obj->tx_dst.y <= 0) {
        ball_obj->sy *= -1;
    }
}

s_game_obj *init_game_obj(int src_x, int src_y,
                          int src_w, int src_h,
                          int dst_x, int dst_y,
                          int dst_w, int dst_h,
                          int sx, int sy) {
    s_game_obj *game_obj = calloc(1, sizeof(s_game_obj));
    game_obj->tx_src.x = src_x;
    game_obj->tx_src.y = src_y;
    game_obj->tx_src.w = src_w;
    game_obj->tx_src.h = src_h;

    game_obj->tx_dst.x = dst_x;
    game_obj->tx_dst.y = dst_y;
    game_obj->tx_dst.w = dst_w;
    game_obj->tx_dst.h = dst_h;

    game_obj->sx = sx;
    game_obj->sy = sy;

    return game_obj;
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
    app_ctx->off_set_x = 0;

    s_game_obj *bg_obj = init_game_obj(130, 385, 30, 30, 0, 0, 30, 30, 0, 0);
    s_game_obj *ball_obj = init_game_obj(260, 490, 22, 22, 12, 12, 12, 12, 5, 3);
    s_game_obj *spaceship = init_game_obj(260, 143, 56, 11,
                                          window_size.w / 2 - 56 / 2, window_size.h - 11,
                                          56, 11,
                                          10, 0);

    bool game_continues = true;
    while (game_continues) {
        handle_events(app_ctx, &game_continues);
        update(app_ctx, ball_obj, spaceship);
        render(app_ctx, bg_obj, ball_obj, spaceship);
    }

    free(app_ctx);
    free(bg_obj);
    free(ball_obj);
    free(spaceship);
}
