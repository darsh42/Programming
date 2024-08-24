#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void handler(int SIG_TYP) {
		static int count = 0;
		signal(SIGINT, handler);
		if (++count == 2) {
				printf("[UPDATE]: Exiting\n");
				exit(0);
		}
}

int main(void) {
		char s[1024];
		signal(SIGINT, handler);
		printf("[UPDATE]: Try hitting C-c twice\n");
		for(;;);
}

