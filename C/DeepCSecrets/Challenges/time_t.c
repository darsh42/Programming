#include <time.h>
#include <stdio.h>

int main() {
    time_t max = 0X7fffFFFFffffFF;
    printf("Max time: %s", asctime(gmtime(&max)));
    time_t cur = time(NULL);
    time_t dif = difftime(max, cur);
    printf("Time Chased by UNIX demon: %s", asctime(gmtime(&dif)));
}
