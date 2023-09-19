#ifndef RAYCASTER_H_INCLUDED
#define RAYCASTER_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>

#define WIDTH  1280
#define HEIGHT 480
#define GAME_WIDTH  640
#define GAME_HEIGHT 480
#define DEBUG_WIDTH  640
#define DEBUG_HEIGHT 480
#define MAP_WIDTH  32
#define MAP_HEIGHT 24


#define START_POSITION {GAME_WIDTH/2.0, GAME_HEIGHT/2.0}
#define FOV 90.0
#define RAYCOUNT GAME_WIDTH
#define LOOKSPEED M_PI/15

bool init();
bool kill();

void render(bool clear);
void draw_vertical_line(double distance, int ray_num, int R, int G, int B);

#endif
