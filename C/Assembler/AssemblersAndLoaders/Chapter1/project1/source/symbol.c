#include "symbol.h"

/* creates a symbol table based on a user defined variable */
int symbol_table_create(struct table *table, uint32_t length) {
    /* already allocated table */
    if (table.entries != NULL)
        return 1;
    
    /* allocate and create the new symbol table */
    table->entries = malloc(sizeof(struct entry) * length);
    table->length  = length;
    table->count   = 0;
}

/* destroys a symbol table based on a user defined variable */
int symbol_table_destroy(struct table *table) {
    /* symbol table is not allocated */
    if (table->entries == NULL)
        return 1;

    free(table->entries);
    table->length = 0;
    table->count  = 0;
}

/* locates symbol in corresponding symbol table */
bool symbol_table_locate(struct table *table, char symbol)
{
    return table->entries[_hash_func(symbol)].symbol == symbol;
}

/* adds symbol to corresponding symbol table */
int  symbol_table_add(struct table *table, enum type type, char symbol, uint32_t value)
{
    /* table full */
    if (table->count == table->length)
        return -1;
    
    /* add the symbol */
    table->entries[_hash_func(symbol)] = (struct entry) {
        .type   = type,
        .symbol = symbol,
        .value  = value
    };

    return 0;
}

int symbol_table_process_label(struct table *table, char *label, int LC) {
    switch (symbol_table_locate(table, *label)) {
        case true:
            /* get entry */
            struct entry e = table->entries[_hash_func(*label)];
            
            /* enumerate based on entry type */
            switch (e.type) {
                case   DEFINED: return e.value;
                case UNDEFINED:
                    /* replace label in source with pointer to previous label refrence */
                    *label = e.value;

                    /* set label value to point to latest label refrence */
                    e.value = LC;
                    return -1;
            }

            break;
        case false:
            return symbol_table_add(table, UNDEFINED, *label, LC);
    }
}
