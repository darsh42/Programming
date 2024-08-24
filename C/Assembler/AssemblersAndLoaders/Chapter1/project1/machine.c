#include <stdio.h>
#include <stdlib.h>

struct machine {
    int ACC;
    int *mem;
} machine;

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: ./machine <instruction type> <assembled source>\n");
        fprintf(stderr, "        instruction-type: uniform, variable\n");
        return 0;
    }
}
