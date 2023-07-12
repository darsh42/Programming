#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// third party libs
#include <SDL2/SDL.h>

// screen dimentions
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// map values
#define MAPW 24
#define MAPH 24

// MACROS
#define SIGN(x) (x < 0) ? -1 : 1
#define MAX(a, b) (a > b) ? a : b
#define MIN(a, b) (a < b) ? a : b
#define MAGNITUDE(x1, y1, x2, y2) sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))
#define ROTATIONMATRIX(angle) {{cos(angle), -sin(angle)}, {sin(angle), cos(angle)}}
#define APPLYROTATION(matix, point) {point.x * matrix[0][0] + point.y * matrix[0][1], point.x * matrix[1][0] + matrix[1][1]}
int map[MAPW][MAPH] =
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
int sector_count;
SDL_Rect *sectors;

// player values
#define player_fov M_PI/3
#define number_of_rays SCREEN_WIDTH
#define playerWidth 12
#define playerHeight 12
#define playerSpeed 3

/* 0 -> x
 * 1 -> y */
typedef struct wall {
	double distance;
	int type;
} wall_t;

SDL_Point mouse_P;
SDL_Point pos_P;

// players distance from walls
wall_t distances[number_of_rays];

/* camera angle
 * range: -PI/2 to PI/2
 * */
double panning_angle = M_PI/100;
double camera_angle = 0;

// player displacement values
SDL_Point move_vector = {0, -1};

// the window to render from
SDL_Window* window = NULL;

// the renderer for the window
SDL_Renderer* renderer = NULL;

// player object
SDL_Rect player;

// functions used in main method
int drawMap();
void drawPlayer();
void handle_input(char inp);

int init() {

	if ((SDL_Init(SDL_INIT_VIDEO)) < 0) {
		printf("Failed to init SDL! SDL_error: %s\n", SDL_GetError());
		return 1;
	}

	window = SDL_CreateWindow("SDL2 window",
							  SDL_WINDOWPOS_UNDEFINED,
							  SDL_WINDOWPOS_UNDEFINED,
							  SCREEN_WIDTH, SCREEN_HEIGHT,
							  SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (window == NULL) {
		printf("could not create SDL window! SDL_error: %s\n", SDL_GetError());
		return 1;
	}



	if ((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
		printf("could not create SDL renderer! SDL_error: %s\n", SDL_GetError());
		return 1;
	}

	pos_P.x = (SCREEN_WIDTH - playerWidth) / 2;
	pos_P.y = (SCREEN_HEIGHT - playerHeight) / 2;
	player.w = playerWidth;
	player.h = playerHeight;

	int wallnum = 0;
    int sector_size[2] = {SCREEN_WIDTH/MAPW, SCREEN_HEIGHT/MAPH};

	// counts number of wall segments for malloc call
    for (int h = 0; h < MAPH; h++) for (int w = 0; w < MAPW; w++) if (map[h][w] != 0) wallnum++;

	sectors = (SDL_Rect *) malloc(wallnum * sizeof(SDL_Rect));
    for (int h = 0; h < MAPH; h++) {
        for (int w = 0; w < MAPW; w++) {
			if (map[h][w] == 0) continue;

            SDL_Rect sector =  {sector_size[0] * w, sector_size[1] * h, sector_size[0], sector_size[1]};
			sectors[sector_count++] = sector;
        }
    }

	return 0;
}

int main(int argc, char* argv[]) {

	if (init() != 0) {printf("failed to initialize application\n"); return 1;}

	bool running = true;
	while(running) {
		SDL_Event event;

		while( SDL_PollEvent(&event) ) {
			switch (event.type) {
				case SDL_QUIT:
					printf("You have quit the application!\n");
					running = false;
					break;
				case SDL_KEYDOWN:
					handle_input(event.key.keysym.sym);
			}
		}

		SDL_SetRenderDrawColor(renderer, 127, 127, 127, SDL_ALPHA_OPAQUE);
		SDL_SetRenderDrawColor(renderer, 180,180,180, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &(SDL_Rect) {0,0,SCREEN_WIDTH,2*(SCREEN_HEIGHT/3)});
		SDL_RenderClear(renderer);

		//drawMap();
		drawPlayer();

		SDL_RenderPresent(renderer);

	}
	// Destroy window
	SDL_DestroyWindow( window );
	// Clean up
	SDL_Quit();
	return 0;
}

void handle_input(char inp) {
	double rotation[2][2] = ROTATIONMATRIX(camera_angle);

	switch (inp) {
		case SDLK_a:
			camera_angle -= panning_angle;
			break;
		case SDLK_d:
			camera_angle += panning_angle;
			break;
		case SDLK_w:
			pos_P = (SDL_Point) {pos_P.x - playerSpeed * (move_vector.x * rotation[0][0] + move_vector.y * rotation[0][1]), pos_P.y + playerSpeed * (move_vector.x * rotation[1][0] + move_vector.y * rotation[1][1])};
			break;
		case SDLK_s:
			pos_P = (SDL_Point) {pos_P.x + playerSpeed * (move_vector.x * rotation[0][0] + move_vector.y * rotation[0][1]), pos_P.y - playerSpeed * (move_vector.x * rotation[1][0] + move_vector.y * rotation[1][1])};
			break;
		default:
			printf("Input key not bound\n");
	}
}

int drawMap() {
	for (int i = 0; i < sector_count; i++) { SDL_SetRenderDrawColor(renderer, 255,255,255, SDL_ALPHA_OPAQUE); SDL_RenderFillRect(renderer, &sectors[i]);}
	return 0;
}

// player utility function, this will draw the rays representing the player
// field of view
void draw_ray(SDL_Point p1, SDL_Point p2, int r, int g, int b);
void sector_collision(SDL_Point point, SDL_Point origin);
void draw_walls();
void drawPlayer() {
	player.x = pos_P.x;
	player.y = pos_P.y;

	SDL_Point center = {pos_P.x,pos_P.y};
	double rot_matrix[2][2] = ROTATIONMATRIX(camera_angle);

	SDL_Point rot_camera = {0 * rot_matrix[0][0] - 10  * rot_matrix[1][0], 0 * rot_matrix[0][1] - 10 * rot_matrix[1][1]};

	SDL_Point camera = {center.x + rot_camera.x, center.y + rot_camera.y};

	//draw_ray(center, camera, 0, 0, 255);
	sector_collision((SDL_Point) {camera.x - center.x, camera.y - center.y}, center);
	draw_walls();
}

void draw_ray(SDL_Point p1, SDL_Point p2, int r, int g, int b) {
	SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
}
bool wall_collision(SDL_Point point) {
	if ((point.x > SCREEN_WIDTH || point.x < 0) | (point.y > SCREEN_HEIGHT || point.y < 0)) return true;

	for (int i = 0; i < sector_count; i++) {
		if ((point.y >= sectors[i].y && point.y <= (sectors[i].y + sectors[i].h)) && ((point.x >= sectors[i].x) && (point.x <= (sectors[i].x + sectors[i].w))))
			return true;
	}

	return false;
}
/* CAN BE ISSUES WITH WALL COLLISION DETECTION */
void sector_collision(SDL_Point vector, SDL_Point origin) {
	if (vector.x == origin.x && vector.y == 0) return;

	int sector_height = SCREEN_HEIGHT / MAPH;
	int sector_width = SCREEN_WIDTH / MAPW;
	int sector_left = ((int) origin.x/sector_width) * sector_width;
	int sector_top = ((int) origin.y/sector_height) * sector_height;
	SDL_Rect sector = {sector_left, sector_top, sector_width, sector_height};

	double magnitude = MAGNITUDE(0, 0, vector.x, vector.y);

	double dx = vector.x/magnitude;
	double dy = vector.y/magnitude;
	double angle = player_fov/number_of_rays;

	for (int i = -(number_of_rays/2), ray_count = 0; i < number_of_rays/2; i++, ray_count++) {
		double rot_matrix[2][2] = ROTATIONMATRIX(angle*i);

		double tdx = dx * rot_matrix[0][0] + dy * rot_matrix[1][0];
		double tdy = dx * rot_matrix[0][1] + dy * rot_matrix[1][1];

		SDL_Point x_intercept = origin;
		SDL_Point y_intercept = origin;

		/* calculates the multiple of the direction vector (dx, dy) to reach the edge of
		   the sector in either direction. When x coord is at the left wall of sector, how many
		   "dy"s need to be added to the start position to reach it.  */
		if (tdx < 0) {
			x_intercept.y += tdy * ((sector.x - origin.x)/ tdx);
			x_intercept.x = sector.x;
			while (!wall_collision(x_intercept)) {
				x_intercept.y -= tdy * (sector.w/tdx);
				x_intercept.x -= sector.w;
			}

		}
		else if (tdx > 0) {
			x_intercept.y += tdy * ((sector.x+sector.w - origin.x) / tdx);
			x_intercept.x = sector.x+sector.w;
			while (!wall_collision(x_intercept)) {
				x_intercept.y += tdy * (sector.w/tdx);
				x_intercept.x += sector.w;
			}
		}
		else {
			while(!wall_collision(x_intercept)) {
				x_intercept.y += SIGN(tdy) * sector.h;
			}
		}
		if (tdy < 0) {
			y_intercept.x += tdx * ((sector.y - origin.y) / tdy);
			y_intercept.y = sector.y;
			while (!wall_collision(y_intercept)) {
				y_intercept.x -= tdx * (sector.h/tdy);
				y_intercept.y -= sector.h;
			}
		}
		else if (tdy > 0) {
			y_intercept.x += tdx * ((sector.y+sector.h - origin.y) / tdy);
			y_intercept.y = sector.y+sector.h;
			while (!wall_collision(y_intercept)) {
				y_intercept.x += tdx * (sector.h/tdy);
				y_intercept.y += sector.h;
			}
		}
		else {
			while(!wall_collision(y_intercept)) {
				y_intercept.x += SIGN(tdx) * sector.w;
			}
		}

		double mag_x = MAGNITUDE(origin.x, origin.y, x_intercept.x, x_intercept.y);
		double mag_y = MAGNITUDE(origin.x, origin.y, y_intercept.x, y_intercept.y);

		if (mag_x<mag_y) {
			distances[ray_count] = (wall_t) {mag_x, 0};
		} else {
			distances[ray_count] = (wall_t) {mag_y, 1};
		}

		SDL_Point point = (mag_x < mag_y) ? x_intercept: y_intercept;

		//draw_ray(origin, point, 0, 0, 255);
	}

	return;
}

void draw_walls() {
	for (int i = 0; i < number_of_rays; i++) {
		double SF = 1000;
		double distance = distances[i].distance;

		SDL_Point center = {i, 320};
		SDL_Point top = {center.x ,center.y - SF*(1/(distance/2))};
		SDL_Point bottom = {center.x , center.y + SF*(1/(distance/2))};

		if (distances[i].type == 0)
			draw_ray(bottom, top, 255,0,0);
		else if (distances[i].type == 1)
			draw_ray(bottom, top, 0,255,0);

	}
}
