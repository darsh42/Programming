/* ========================================================================= *
 *                             Commands to link                              *
 * ========================================================================= *
 * COMPILE LIBRARY:                                                          *
 * gcc -c -fpic <library.c>                                                  *
 * gcc -shared -o lib<library>.so <library.o>                                *
 *                                                                           *
 * COMPILE PROGRAM:                                                          *
 * gcc -L<path to lib> -R<path to lib> -o <bin> <program files> -llibrary    *
 *                                                                           *
 * !NOT RECOMMENDED SET LIBRARY PATH:                                        *
 * export LD_LIBRARY_PATH=<path to library>:$LD_LIBRARY_PATH                 *
 *                                                                           *
 * EXECUTION:                                                                *
 * ./program <args>                                                          *
 * ========================================================================= *
 *                                EXAMPLE                                    *
 * ========================================================================= *
 * The shared library and main program are in the same dire-                 *
 * tory                                                                      *
 *                                                                           *
 * gcc -o -fpic fruits.c                                                     *
 * gcc -c -shared libfruits.so fruits.c                                      *
 * gcc -L. -R. -o main main.c -lfruits                                       *
 * ./main                                                                    *
 * ========================================================================= *
 *                                  END                                      *
 * ========================================================================= */
