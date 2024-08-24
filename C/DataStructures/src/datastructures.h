#ifndef DATASTRUCTURES_H_INCLUDED
#define DATASTRUCTURES_H_INCLUDED

#include "common.h"

typedef struct FIFO {
    int *array, size, len;
    
    int head, tail;
} fifo_t;
typedef struct DYNAMIC_ARRAY {
    const char **array;
    int length;
    int size;
} darray_t;
typedef struct LINKED_LIST_NODE ll_node_t;
typedef struct BINARY_TREE_NODE bt_node_t;

extern bool fifo_full(fifo_t fifo);
extern bool fifo_empty(fifo_t fifo);
extern int fifo_push(fifo_t *fifo, int elem);
extern int fifo_peek(fifo_t fifo);
extern int fifo_pop(fifo_t *fifo);
extern void fifo_create(fifo_t *fifo, int size);
extern void fifo_destroy(fifo_t *fifo);
extern void fifo_print(fifo_t fifo);

extern darray_t *darray_create(int size);
extern void darray_destroy(darray_t *da);
extern darray_t *darray_resize(darray_t *da, int size);
extern void darray_insert(darray_t *da, const char *item);
extern const char *darray_remove(darray_t *da);
extern const char **darray_contents(darray_t *da);

extern void ll_prepend(ll_node_t **head, int value);
extern void ll_append(ll_node_t **head, int value);
extern void ll_insert(ll_node_t **head, int value, int index);
extern void ll_remove_first(ll_node_t **head);
extern void ll_remove_last(ll_node_t **head);
extern void ll_remove_index(ll_node_t **head, int index);
extern void ll_remove_find(ll_node_t **head, int value);
extern void ll_destroy(ll_node_t *head);
extern void ll_print(ll_node_t *head);
extern int ll_find(ll_node_t *head, int value);

extern bt_node_t *BSTinsert(bt_node_t *root, int value);
extern void BSTremove(bt_node_t *root, int value);
extern void BSTdelete(bt_node_t *root);
extern void BSTsearch(bt_node_t *root, bt_node_t *result, int value);
extern void BSTdisplay(bt_node_t *root, int space);

#endif // DATASTRUCTURES_H_INCLUDED
