/* ================================================= *
 *         INVESTIGATION: const qualifier            *
 * ================================================= *
 * The const qualifier is used when making a variab- *
 * le immutable.This is does not mean the undelying  *
 * data is immutable, but rather that it cannot be   *
 * mutated from the const qualified variable.        *
 *                                                   *
 * A further point is that an const int can hold an  *
 * int, but assigning a const int to an int will th- *
 * row an error. This error is that the const quali- *
 * fier is dropped. In general assigning a qualified *
 * expression to a non-qualified expression will th- *
 * row this error.                                   *
 *                                                   *
 * The const qualifier should be known as "readonly" *
 *                                                   *
 * QUALIFIED: 'const', 'volitile' and 'restrict'     *
 * ================================================= */


/*
** EXAMPLES
*/
#include <stdio.h>

void foo(int *cur, int *new) {
    *cur = *new;
}

/* const qualifiers in function prototypes are    *
 * used to ensure that the value is not changed   *
 * calling code. This practice is only used in    *
 * pointers as parameters are passed by value and *
 * not reference                                  */
void baz(const int *cur, int *new) {
    *cur = *new;
}

int main(int arg, char **argv) {
    const int *p;
    const int x;
    int a, b;
    a = 1;
    b = 2;
    /* the pointer can be changed */
    p = &a;
    p = &b;
    /* the value pointed to by the pointer cannot */
    *p = b;
    /* the const int cannot be changed */
    x = a;
    printf("%d\n", *p);
    printf("%d\n", *p);

    // to make a pointer read only:
    int const *q;
    int *const r;
    // to make both the pointer and object pointed to const
    const int * const s;
    int const * const t;
}
