#include <signal.h>
#include <stdlib.h>
#include <ncurses.h>

void handler(int type);

int main(void) {
    signal(SIGINT, handler);

    while (1)
        ;
}

void handler(int type) {
    if (type == SIGINT) {
        printf("Interrupted");
        exit(0);
    }
}
