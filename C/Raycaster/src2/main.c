#include <math.h>
#include <raylib.h>
#define WIDTH 640
#define HEIGHT 480

#define MAP_WIDTH  16
#define MAP_HEIGHT  12

#define SECTOR_W (double) WIDTH / MAP_WIDTH
#define SECTOR_H (double) HEIGHT / MAP_HEIGHT

#define ABS(a) (a < 0) ? -a: a

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
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};


int render_map() {
    BeginDrawing();

    for (int mapY = 0; mapY < MAP_HEIGHT; mapY++) {
        for(int mapX = 0; mapX < MAP_WIDTH; mapX++) {
            if (map[mapY][mapX] == 0) continue;

            int w = WIDTH / MAP_WIDTH;
            int h = HEIGHT / MAP_HEIGHT;
            int x = w * mapX;
            int y = h * mapY;

            DrawRectangle(x, y, w, h, BLUE);
        }
    }

    EndDrawing();
    return 0;
}

int raycast() {
    // initialize player variables
    player.pos_x = MAP_WIDTH / 2.0;
    player.pos_y = MAP_HEIGHT / 2.0;

    player.dir_x = 0;
    player.dir_y = 1;

    /* Find the edge of the map in the direction the player is looking in
     * if the player look direction is positive they are looking into the next sector */

    int sector_x = (int) player.pos_x + ((player.dir_x < 0) ? 0: 1);
    int sector_y = (int) player.pos_y + ((player.dir_y < 0) ? 1: 0);
    double delta_x = ABS(sector_x - player.pos_x);
    double delta_y = ABS(sector_y - player.pos_y);

    // how much the x or y line needs to be moved after every step
    double scaling_x_line = (1.0 / player.dir_x) * player.dir_y;
    double scaling_y_line = (1.0 / player.dir_y) * player.dir_x;

    scaling_x_line = (scaling_x_line > MAP_HEIGHT) ? 0: scaling_x_line;
    scaling_y_line = (scaling_y_line > MAP_WIDTH) ? 0: scaling_y_line;

    // each line has current x and y
    int x_line = sector_x;
    // scale based on ratio between dir vec and distance from sector edge
    double x_line_y = (player.dir_x / delta_x) * player.dir_y;

    int y_line = sector_y;
    double y_line_x = (player.dir_y / delta_y) * player.dir_x;

    x_line_y = ((ABS(x_line_y)) == NAN) ? 0: x_line_y;
    y_line_x = ((ABS(y_line_x)) == NAN) ? 0: y_line_x;

    // while the x and y of each line has not hit a sector containing a wall keep stepping
    while(map[(int) x_line_y][x_line] == 0 || map[y_line][(int) y_line_x] == 0) {
        // increment to next sector and add corresponding y or x increase to line
        x_line++;
        x_line_y += scaling_x_line;

        y_line++;
        y_line_x += scaling_y_line;
    }

    DrawLine(SECTOR_W*player.pos_x, SECTOR_H*player.pos_y, SECTOR_W*x_line, SECTOR_H*x_line_y, GREEN);
    DrawLine(SECTOR_W*player.pos_x, SECTOR_H*player.pos_y, SECTOR_W*y_line_x, SECTOR_H*y_line, RED);

    return 0;
}

int main() {
    InitWindow(WIDTH, HEIGHT, "Raycaster");

    render_map();
    BeginDrawing();

    raycast();

    EndDrawing();
    WaitTime(40);

    if (WindowShouldClose())
        CloseWindow();

    return 0;
}
