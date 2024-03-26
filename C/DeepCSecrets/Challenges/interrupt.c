#include <signal.h>
#include <ncurses.h>

void handler(int type);

int main(void) {

}

void handler(int type) {
		signal(handler, SIGPOLL);
}
