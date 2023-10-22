#include "raycaster.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define mapWidth 32
#define mapHeight 24
#define screenWidth 640
#define screenHeight 480

int worldMap[mapHeight][mapWidth]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int main() {
        
    double posX = 22, posY = 12;
    double dirX = -1, dirY = 1;
    double planeX = 0, planeY = 1;
    double old_plane_X, old_dir_X;

    double time = 0;
    double oldTime = 0;
    
    bool quit = false;
    
    init();
    while(!quit) {
        render(true);
        for(int x = 0; x < GAME_WIDTH; x++) {
            double cameraX = 2.0 * x / GAME_WIDTH - 1;
            double rayDirX = dirX + planeX * cameraX;
            double rayDirY = dirY + planeY * cameraX;

            int mapX = (int) posX;
            int mapY = (int) posY;

            double sideDistX;
            double sideDistY;

            double deltaDistX = (rayDirX == 0) ? 1E30 : fabs(1 / rayDirX);
            double deltaDistY = (rayDirY == 0) ? 1E30 : fabs(1 / rayDirY);

            int stepX;
            int stepY;

            int hit = 0;
            int side;

            if (rayDirX < 0) {
                stepX = -1;
                sideDistX = (posX - mapX) * deltaDistX;
            } else {
                stepX = 1;
                sideDistX = (mapX + 1.0 - posX) * deltaDistX;
            }

            if (rayDirY < 0) {
                stepY = -1;
                sideDistY = (posY - mapY) * deltaDistY;
            } else {
                stepY = 1;
                sideDistY = (mapY + 1.0 - posY) * deltaDistY;
            }

            while (hit == 0) {
                if (sideDistX < sideDistY) {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                } else {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }

                if (worldMap[mapX][mapY] > 0) hit = 1;
            }
            
            double perpWallDist;
            if (side == 0) perpWallDist = (sideDistX - deltaDistX);
            else           perpWallDist = (sideDistY - deltaDistY);

            int lineHeight = (int) GAME_HEIGHT/perpWallDist;

            if (side == 0) draw_vertical_line(lineHeight, x, 128, 128, 128);
            else           draw_vertical_line(lineHeight, x, 255, 255, 255);
        }

        // draw_debug(posX, posY, dirX, dirY);
        draw_crosshair();

        render(false);
        SDL_Event event;

        oldTime = time;
        time = clock();
        double frame_time = (time - oldTime) / 1000.0;

        double movespeed = frame_time;
        double rotspeed = 0.0314;

        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case(SDL_QUIT): quit = true; break;
                case(SDL_KEYDOWN):
                    switch (event.key.keysym.sym) {
                        case(SDLK_q): quit = true; break;
                        case(SDLK_w):
                          if (worldMap[(int) (posX + dirX * movespeed)][(int) posY] == 0) posX += dirX * movespeed;
                          if (worldMap[(int) posX][(int) (posY + dirY * movespeed)] == 0) posY += dirY * movespeed;
                          break;
                        case(SDLK_s):
                          if (worldMap[(int) (posX - dirX * movespeed)][(int) posY] == 0) posX -= dirX * movespeed;
                          if (worldMap[(int) posX][(int) (posY - dirY * movespeed)] == 0) posY -= dirY * movespeed;
                          break;
                        case(SDLK_d):
                          old_dir_X = dirX;
                          dirX = dirX * cos(-rotspeed) - dirY * sin(-rotspeed);
                          dirY = old_dir_X * sin(-rotspeed) + dirY * cos(-rotspeed);

                          old_plane_X = planeX;
                          planeX = planeX * cos(-rotspeed) - planeY * sin(-rotspeed);
                          planeY = old_plane_X * sin(-rotspeed) + planeY * cos(-rotspeed);
                          break;
                        case(SDLK_a):
                          old_dir_X = dirX;
                          dirX = dirX * cos(rotspeed) - dirY * sin(rotspeed);
                          dirY = old_dir_X * sin(rotspeed) + dirY * cos(rotspeed);

                          old_plane_X = planeX;
                          planeX = planeX * cos(rotspeed) - planeY * sin(rotspeed);
                          planeY = old_plane_X * sin(rotspeed) + planeY * cos(rotspeed);
                          break;
                    }
            }
        }

        //SDL_Delay((int)(16 - (clock() - time)));
    }


    kill();

    return 0;
}

SDL_Window *win;
SDL_Renderer *rend;
SDL_Texture *scr;
uint8_t screen[GAME_WIDTH * GAME_HEIGHT * 4] = {0};
int center = GAME_HEIGHT/2;

bool init() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "graphics.c: Error could not init SDL, SDL_error: %s\n", SDL_GetError());
        return false;
    }

    if ((win = SDL_CreateWindow("RAYCASTER",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                GAME_WIDTH, GAME_HEIGHT,
                                SDL_WINDOW_SHOWN)) == NULL) {
        fprintf(stderr, "raycaster.c: init() => Error could not make window, SDL_error: %s\n", SDL_GetError());
        return false;
    }

    if ((rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
        fprintf(stderr, "raycaster.c: init() => Error could not make renderer, SDL_error: %s\n", SDL_GetError());
        return false;
    }

    if ((scr = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, GAME_WIDTH, GAME_HEIGHT)) == NULL) {
      fprintf(stderr, "raycaster.c: init() => Error could not create screen texture, SDL_Error: %s\n", SDL_GetError());
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
    if (clear) {
        SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(rend);
    }
    else SDL_RenderPresent(rend);

    return;
}

void update_screen() {
    int texture_pitch = 0;
    void *texture_pixels = NULL;

    if (SDL_LockTexture(scr, NULL, &texture_pixels, &texture_pitch) != 0) {
        fprintf(stderr, "raycaster.c: update_screen() => Error could not lock texture: 'scr', SDL_Error: %s\n", SDL_GetError());
        return;
    } else {
      memcpy(texture_pixels, screen, texture_pitch * GAME_HEIGHT);
    }

    SDL_UnlockTexture(scr);
}

// renders each vertical line in the raycaster loop
void draw_vertical_line(double line_height, int ray_num, int R, int G, int B) {
    /*
     * "30/distance" --> smaller distance == bigger line, larger distance == smaller line
     */
    //printf("Line distance: {RAYNUM: %d, PERPDISTANCE: %f}\n", ray_num, fabs(line_height/2));
    SDL_SetRenderDrawColor(rend, R, G, B, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(rend, ray_num, center - line_height/2, ray_num, center + line_height/2);

     //int top = center - line_height/2, bottom = center + line_height/2;
     //for (int pixel = ray_num + top * GAME_WIDTH * 4; pixel <= ray_num + bottom * GAME_WIDTH * 4; pixel += ray_num + GAME_WIDTH * 4) {
     //    screen[pixel + 0] = R;
     //    screen[pixel + 1] = G;
     //    screen[pixel + 2] = B;
     //    screen[pixel + 3] = 0;
     //}

    return;
}

void draw_crosshair() {
  SDL_SetRenderDrawColor(rend, 0, 255, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(rend, GAME_WIDTH/2 - 10, center, GAME_WIDTH/2 + 10, center);
  SDL_RenderDrawLine(rend, GAME_WIDTH/2, center + 10, GAME_WIDTH/2, center - 10);
}

void draw_debug(double posX, double posY, double dirX, double dirY) {
    int x_step = DEBUG_WIDTH/MAP_WIDTH, y_step = DEBUG_HEIGHT/MAP_HEIGHT;

    for (int i = 0, y = 0; i < mapHeight; i++, y += y_step) {
        for (int j = 0, x = 0; j < mapWidth; j++, x += x_step) {
            SDL_Rect sector = {x, y, x_step, y_step};
            if (worldMap[i][j] != 0)
                SDL_SetRenderDrawColor(rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
            else
                SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(rend, &sector);
        }
    }

    SDL_SetRenderDrawColor(rend, 0, 0, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(rend, posX * GAME_WIDTH, posY * GAME_HEIGHT);

    SDL_SetRenderDrawColor(rend, 0, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(rend, posX * GAME_WIDTH, posY * GAME_HEIGHT, (posX + dirX) * GAME_WIDTH, (posY + dirY) * GAME_HEIGHT);
}
