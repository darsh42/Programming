#include "../scientific.h"

int newton_raphson(int xn, int fx, int dx) {
    return xn - fx/dx;
}
