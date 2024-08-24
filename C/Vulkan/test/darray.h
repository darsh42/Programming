#ifndef DARRAY_H_INCLUDED
#define DARRAY_H_INCLUDED

typedef struct DYNAMIC_ARRAY {
    const char **array;
    int length, size;
} darray_t;

extern darray_t *darray_create(int size);
extern void darray_destroy(darray_t *da);
extern darray_t *darray_resize(darray_t *da, int size);
extern void darray_insert(darray_t *da, const char *item);
extern void darray_insert_bulk(darray_t *da, const char **content, int count);
extern const char *darray_remove(darray_t *da);
extern const char **darray_contents(darray_t *da);
extern int darray_length(darray_t *da);

#endif // !DARRAY_H_INCLUDED
