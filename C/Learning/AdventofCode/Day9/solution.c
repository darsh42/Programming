#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#define LINEBUF 10

typedef struct Point {
    int x;
    int y;
} point_t;

void move(char direction, int steps, point_t *point);
bool tail_proximity(point_t head, point_t tail);

int main(int argc, char *argv[]) {
    if (argc != 2) {printf("Usage: ./solution input.txt\n"); return 1;}

    int part1 = 1;
    int part2 = 1;

    // starting positions
    point_t head = {0,0};
    point_t tail = {0,0};

    point_t prev_head = {0,0};

    char line[LINEBUF];
    FILE *fp = fopen(argv[1], "r");
    while ( fgets(line, LINEBUF, fp) != NULL ) {
        prev_head = head;
        move(line[0], atoi(&line[2]), &head);
        if (!tail_proximity(head, tail)) {
            tail = prev_head;
            part1++;

        }
    }
    printf("part1: %d\n", part1);

    return 0;
}

int distance(point_t p1, point_t p2) {
    return sqrt( pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2) );
}

void move(char direction, int steps, point_t *point) {
    switch (direction) {
        case ('U'):
            point->y += steps;
            break;
        case ('D'):
            point->y -= steps;
            break;
        case ('L'):
            point->x -= steps;
            break;
        case ('R'):
            point->x += steps;
            break;
        default:
            printf("Error| unknown direction: %c!\n", direction);
    }
}

/* check if head is within 3x3 kernel around tail */
bool tail_proximity(point_t head, point_t tail) {
    if (head.x >= tail.x-1 && head.x <= tail.x+1 && head.y >= tail.y-1 && head.y <= tail.y+1)
        return true;

    return false;
}
