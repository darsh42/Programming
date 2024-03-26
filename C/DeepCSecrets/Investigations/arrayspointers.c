/* ================================================= *
 *        INVESTIGATION: arrays vs pointers          *
 * ================================================= *
 * The general idea is that arrays and pointers are  *
 * equivalent. This is however not the case in all   *
 * instances.                                        *
 *                                                   *
 * The difference between arrays and pointers are m- *
 * ainly in their definition, specifically what the  *
 * definition means.                                 *
 *                                                   *
 * The array definition is simply an alias for a me- *
 * mory address, the pointer is a variable assignme- *
 * nt pointing to a different piece of memory.       *
 *                                                   *
 * The distinction is important as the array start   *
 * location will be known at compile time but the v- *
 * alue of a given pointer will be known at run time *
 *                                                   *
 * This leads to the following performance "modifie- *
 * r", arrays take one less fetch instruction compa- *
 * red to the pointers.                              *
 * ================================================= */

/* ===================Scenario 1=====================*
 * Array created in file1.c will not link with extern*
 * pointer referring to array in file2.c             */
// file1.c
char mango[100]; // array defined
// file2.c
extern char *mango; // pointer to link

/* This will not compile in a project as the array   *
 * and pointer are seen as 2 differing prototypes (  *
 * see notes above).                                 */
