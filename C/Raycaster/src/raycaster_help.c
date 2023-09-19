#include "raycaster.h"
#define mapWidth 24
#define mapHeight 24
#define screenWidth 640
#define screenHeight 480

int worldMap[mapWidth][mapHeight]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int main() {
        
    double posX = 22, posY = 12;
    double dirX = -1, dirY = 1;
    double planeX = 0, planeY = 0.66;

    double time = 0;
    double oldTime = 0;
    
    bool quit = false;
    
    init();
    while(!quit) {
        render(true);
        for(int x = 0; x < GAME_WIDTH; x++) {
            double cameraX = 2 * x / GAME_WIDTH - 1;
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

            if (side == 0) draw_vertical_line(lineHeight, x, 0, 0, 255);
            else           draw_vertical_line(lineHeight, x, 0, 255, 0);
        }
        
        render(false);
        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case(SDL_QUIT): quit = true; break;
                        case(SDLK_q): quit = true; break;                    
            }
        }
    }

    
    kill();

    return 0;
}

SDL_Window *win;
SDL_Renderer *rend;
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

// renders each vertical line in the raycaster loop
void draw_vertical_line(double line_height, int ray_num, int R, int G, int B) {
    /*
     * "30/distance" --> smaller distance == bigger line, larger distance == smaller line
     */
    //printf("Line distance: {RAYNUM: %d, PERPDISTANCE: %f}\n", ray_num, fabs(distance));
    SDL_SetRenderDrawColor(rend, R, G, B, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(rend, ray_num, center - line_height/2, ray_num, center + line_height/2);
    return;
}
