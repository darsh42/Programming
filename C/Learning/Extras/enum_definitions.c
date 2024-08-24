#include <stdio.h>

enum ENUM {
    ENABLE = 0,
    DISABLE = !ENABLE,
};

int main(void) {
    printf("ENABLE: %d\n", ENABLE);
    printf("DISABLE: %d\n", DISABLE);
    return 0;
}
