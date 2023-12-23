#include "SDL2/SDL.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#define WIDTH 640
#define HEIGHT 480

#define MAP_WIDTH  16
#define MAP_HEIGHT  9

#define ABS(a) (a < 0) ? -a: a;


struct {
    // range 0 - max width or height respective
    double pos_x;
    double pos_y;

    // range -1 to 1, y = 1 -> up, x = 1 -> right
    int dir_x;
    int dir_y;
} player;


int map[MAP_HEIGHT][MAP_WIDTH] =
{
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

SDL_Window *win;
SDL_Renderer *rend;

int main(int argc, char **argv) {
    int status;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "[Error] main.c: Cannot start SDL context, SDL_Error: %s", SDL_GetError());
        return 1;
    }

    if ((win = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_CENTERED_MASK, SDL_WINDOWPOS_CENTERED_MASK, WIDTH, HEIGHT, SDL_WINDOW_SHOWN)) == NULL) {
        fprintf(stderr, "[Error] main.c: Cannot create window, SDL_Error: %s", SDL_GetError());
        return 1;
    }

    if ((rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
        fprintf(stderr, "[Error] main.c: Cannot create renderer, SDL_Error: %s", SDL_GetError());
        return 1;
    }

    SDL_SetRenderDrawColor(rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(rend);
    SDL_SetRenderDrawColor(rend, 100, 100, 100, SDL_ALPHA_OPAQUE);
    for (int mapY = 0; mapY < MAP_HEIGHT; mapY++) {
        for(int mapX = 0; mapX < MAP_WIDTH; mapX++) {
            if (map[mapY][mapX] == 0) continue;

            int w = WIDTH / MAP_WIDTH;
            int h = HEIGHT / MAP_HEIGHT;
            int x = w * mapX;
            int y = h * mapY;
            SDL_Rect sector = {x, y, w, h};
            SDL_RenderFillRect(rend, &sector);
        }
    }

    // initialize player variables
    player.pos_x = MAP_WIDTH / 2.0;
    player.pos_y = MAP_HEIGHT / 2.0;

    player.dir_x = 0;
    player.dir_y = 1;

    /* Find the edge of the map in the direction the player is looking in
     * if the player look direction is positive they are looking into the next sector */

    int sector_x = (int) player.pos_x + ((player.dir_x < 0) ? 0: 1);
    int sector_y = (int) player.pos_y + ((player.dir_y < 0) ? 0: 1);
    double delta_x = ABS(sector_x - player.pos_x);
    double delta_y = ABS(sector_y - player.pos_y);

    // how much the x or y line needs to be moved after every step
    double scaling_x_line = (1 / player.dir_x) * player.dir_y;
    double scaling_y_line = (1 / player.dir_y) * player.dir_x;

    // each line has current x and y
    int x_line = sector_x;
    // scale based on ratio between dir vec and distance from sector edge
    double x_line_y = (player.dir_x / delta_x) * player.dir_y;

    int y_line = sector_y;
    double y_line_x = (player.dir_y / delta_y) * player.dir_x;

    // while the x and y of each line has not hit a sector containing a wall keep stepping
    while(map[(int) x_line_y][x_line] == 0 || map[y_line][(int) y_line_x] == 0) {
        // increment to next sector and add corresponding y or x increase to line
        x_line++;
        x_line_y += scaling_x_line;

        y_line++;
        y_line_x += scaling_y_line;
    }

    SDL_Delay(1000);
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(rend);
    SDL_Quit();

    return 0;
}
