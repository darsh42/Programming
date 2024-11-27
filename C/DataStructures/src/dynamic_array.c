#include "datastructures.h"

darray_t *darray_create(int size) {
    darray_t *da = malloc(sizeof (darray_t));
    
    da->array  = malloc(size * sizeof (const char *));
    da->length = 0;
    da->size   = size;
    return da;
}

void darray_destroy(darray_t *da) {
    free(da->array);
    free(da);
}

darray_t *darray_resize(darray_t *da, int size) {
    const char **resize = realloc(da->array, size * sizeof (const char *));

    if (resize == NULL) {
        darray_destroy(da);
        fprintf(stderr, "[ERROR]: Could not size dynamic array, realloc failed\n");
        exit(-1);
    }
    da->array = resize;
    da->size  = size;
    return da;
}

void darray_insert(darray_t *da, const char *item) {
    if (da->length == da->size) {
        darray_resize(da, da->size + da->size);
    }

    da->array[da->length++] = item;
}

const char *darray_remove(darray_t *da) {
    if (da->length <= da->size/4) {
        darray_resize(da, da->size/2);
    }

    return (da->length == 0) ? NULL : da->array[--da->length];
}

const char **darray_contents(darray_t *da) {
    return da->array;
}