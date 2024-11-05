#ifndef SYMBOL_INCLUDED_H
#define SYMBOL_INCLUDED_H

/***********************************************************************************************
 *                                                                                             *
 *                                      SYMBOL TABLE                                           *
 *                                                                                             *
 ***********************************************************************************************/

#include <stdout.h>
#include <stdint.h>

#define _hash 26
#define _hash_func(index) index % _hash

/***********************************************************************************************
 *                                                                                             *
 *                                       STRUCTURES                                            *
 *                                                                                             *
 ***********************************************************************************************/

struct table;
struct entry;

enum type {
    UNDEFINED = 0,
    DEFINED   = 1
};

/* table structure */
struct table {
    struct entry *entries;      /* entries within the symbol table         */
    
    uint32_t length;            /* total max length of the symbol table    */
    uint32_t count;             /* current number of entries used in table */
};

/* entry structure */
struct entry {
    enum type type;             /* symbol type, see enum definition        */

    char     symbol;            /* identifier of symbol stored             */
    uint32_t value;             /* value represented by symbol             */
};

/***********************************************************************************************
 *                                                                                             *
 *                                        FUNCTIONS                                            *
 *                                                                                             *
 ***********************************************************************************************/


#endif // SYMBOL_INCLUDED_H
