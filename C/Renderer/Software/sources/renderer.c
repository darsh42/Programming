#include "renderer.h"

int main( void ) 
{
    struct triangle cube[] = {
        /* top */
        {{0, 1, 0}, {0, 1, 1}, {1, 1, 1}},
        {{0, 1, 0}, {1, 1, 0}, {1, 1, 1}},

        /* north */
        {{0, 0, 1}, {0, 1, 1}, {1, 1, 1}},
        {{0, 0, 1}, {1, 0, 1}, {1, 1, 1}},

        /* west */
        {{0, 0, 0}, {0, 1, 1}, {0, 1, 1}},
        {{0, 0, 0}, {1, 0, 1}, {0, 1, 1}},

        /* south */
        {{0, 0, 0}, {0, 1, 0}, {1, 1, 0}},
        {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}},
        
        /* east */
        {{1, 0, 0}, {0, 1, 1}, {1, 1, 0}},
        {{1, 0, 0}, {1, 0, 1}, {1, 1, 0}},
        
        /* bottom */
        {{0, 0, 0}, {0, 0, 1}, {1, 0, 1}},
        {{0, 0, 0}, {1, 0, 0}, {1, 0, 1}},
    };

    struct vertex_matrix = 

    return 0;
}