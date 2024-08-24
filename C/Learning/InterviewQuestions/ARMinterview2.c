#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc == 1) {fprintf(stderr, "Usage: ./ARM2 <number>\n"); return 1;}

    int bit_twin = 0;
    for (int num = atoi(*(argv+1)); num != 0; num >>=1) {
        if ((num & 0b1111) == 0b0110) {
            bit_twin++;
            num >>= 3;
        }
    }
    printf("bit_twins: %d\n", bit_twin);

    return 0;
}
