#include "raycaster.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>

int map[MAP_HEIGHT][MAP_WIDTH] = {
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

typedef struct vector {
    double x;
    double y;
} vec_t;

double mag(vec_t vector) {
    return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}
double abs_double(double x) {
    return (x < 0) ? x * -1 : x;
}
vec_t add_const(vec_t vector, double constant) {
    return (vec_t) {vector.x + constant, vector.y + constant};
}
vec_t mul_const(vec_t vector, double constant) {
    return (vec_t) {vector.x * constant, vector.y * constant};
}
vec_t add_vec(char sign, int num, ...) {
    vec_t result = {0, 0};
    va_list vectors;

    va_start(vectors, num);

    for (int i = 0; i < num; i++) {
        vec_t tmp = va_arg(vectors, vec_t);

        result.x += tmp.x * ((sign == '-' && i != 0) ? -1: 1);
        result.y += tmp.y * ((sign == '-' && i != 0) ? -1: 1);
    }

    return result;
}
vec_t mul_vec(int num, ...) {
    vec_t result = {0, 0};
    va_list vectors;

    va_start(vectors, num);

    for (int i = 0; i < num; i++) {
        result.x *= va_arg(vectors, vec_t).x;
    }

    for (int i = 0; i < num; i++) {
        result.y *= va_arg(vectors, vec_t).y;
    }

    return result;
}
vec_t rot_vec(vec_t vector, double angle) {
    vec_t rot = {vector.x * cos(angle) + vector.y * sin(angle),
                 vector.x * -sin(angle) + vector.y * cos(angle)};
    return rot;
}

void draw_debug(vec_t player_pos, vec_t player_dir);

int main(void) {
    init();
    bool quit = false;

    // pos -> player position
    // dir -> direction vector
    // plane -> size of camera plane
    vec_t pos = START_POSITION;
    vec_t dir = {0, -10};
    vec_t plane = {0, mag(dir) * tan(FOV/2)};

    while(!quit) {
        render(true);
        for (int i = -RAYCOUNT/2; i < RAYCOUNT/2; i++) {
            vec_t ray = add_vec('+', 3, pos, dir, mul_const(plane, (2.0*i)/RAYCOUNT));

            vec_t sector_edge, step;
            vec_t sector_map = {(int) pos.x, (int) pos.y};
            vec_t delta_dist = (vec_t) {abs_double(1/ray.x), abs_double(1/ray.y)};

            sector_map = (vec_t) {(int)ray.x, (int) ray.y};

            if (dir.x < 0) {
                step.x = -1;
                sector_edge.x = pos.x - sector_map.x;
            } else {
                step.x = 1;
                sector_edge.x = sector_map.x + 1.0 - pos.x;
            }

            if (dir.y < 0) {
                step.y = -1;
                sector_edge.y = pos.y - sector_map.y;
            } else {
                step.y = 1;
                sector_edge.y = sector_map.y + 1.0 - pos.y;
            }

            bool hit = false;
            int side; // NS or EW hit

            while (!hit) {
                if (delta_dist.x < delta_dist.y) {
                    sector_edge.x += delta_dist.x;
                    sector_map.x += step.x;
                    side = 0;
                } else {
                    sector_edge.y += delta_dist.y;
                    sector_map.y += step.y;
                    side = 1;
                }

                if (map[(int)sector_map.x][(int)sector_map.y] != 0) hit = true;
            }

            int perp_wall_distance;
            if (side == 0) perp_wall_distance = sector_edge.x - delta_dist.x;
            if (side == 1) perp_wall_distance = sector_edge.y - delta_dist.y;

            //if (side == 0)
                //draw_vertical_line(perp_wall_distance, i + RAYCOUNT/2, 255, 0, 0);
            //else
                //draw_vertical_line(perp_wall_distance, i + RAYCOUNT/2, 0, 0, 255);
        }

        draw_debug(pos, dir);

        render(false);

        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case(SDL_QUIT): quit = true; break;
                case(SDL_KEYDOWN):
                    switch (event.key.keysym.sym) {
                        case(SDLK_w):
                            if (map[(int) ((pos.y + dir.y)) / MAP_HEIGHT][(int) ((pos.x + dir.x) / MAP_WIDTH)] == 0)
                                pos = add_vec('+', 2, pos, dir);
                            break;
                        case(SDLK_s):
                            if (map[(int) ((pos.y + dir.y)) / MAP_HEIGHT][(int) ((pos.x + dir.x) / MAP_WIDTH)] == 0)
                                pos = add_vec('-', 2, pos, dir);
                            break;
                        case(SDLK_a): dir = rot_vec(dir, LOOKSPEED); break;
                        case(SDLK_d): dir = rot_vec(dir, -LOOKSPEED); break;
                        case(SDLK_q): quit = true; break;
                    }
            }
        }

        printf("player pos: (%f, %f)\n", pos.x, pos.y);
        printf("player dir: (%f, %f)\n", dir.x, dir.y);
    }

    kill();
    return 0;
}


SDL_Window *win;
SDL_Renderer *rend;
vec_t center_point = (vec_t) {WIDTH/2.0, HEIGHT/2.0};

bool init() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "graphics.c: Error could not init SDL, SDL_error: %s\n", SDL_GetError());
        return false;
    }

    if ((win = SDL_CreateWindow("RAYCASTER",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                WIDTH, HEIGHT,
                                SDL_WINDOW_SHOWN)) == NULL) {
        fprintf(stderr, "raycaster.c: init() => Error could not make window, SDL_error: %s\n", SDL_GetError());
        return false;
    }

    if ((rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
        fprintf(stderr, "raycaster.c: init() => Error could not make renderer, SDL_error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

bool kill() {
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(rend);
    SDL_Quit();
    return true;
}

void render(bool clear) {
    if (clear) {SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE); SDL_RenderClear(rend);}
    else       SDL_RenderPresent(rend);

    return;
}

void draw_vertical_line(double distance, int ray_num, int R, int G, int B) {
    vec_t line[2] = {{ray_num, center_point.y + 30/distance},
                     {ray_num, center_point.y - 30/distance}};
    SDL_SetRenderDrawColor(rend, R, G, B, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(rend, line[0].x, line[0].y, line[1].x, line[1].y);
    return;
}

void draw_debug(vec_t player_pos, vec_t player_dir) {
    int x_step = WIDTH/MAP_WIDTH, y_step = HEIGHT/MAP_HEIGHT;

    for (int i = 0, y = 0; i < MAP_HEIGHT; i++, y += y_step) {
        for (int j = 0, x = 0; j < MAP_WIDTH; j++, x += x_step) {
            SDL_Rect sector = {x, y, x_step, y_step};
            if (map[i][j] != 0)
                SDL_SetRenderDrawColor(rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
            else
                SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(rend, &sector);
        }
    }

    SDL_SetRenderDrawColor(rend, 0, 0, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(rend, player_pos.x, player_pos.y);

    SDL_SetRenderDrawColor(rend, 0, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(rend, player_pos.x, player_pos.y, player_pos.x + player_dir.x, player_pos.y + player_dir.y);

}
