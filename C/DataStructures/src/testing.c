#include "datastructures.h"
#include <assert.h>

void FIFOtest(void) {
    fifo_t fifo;
    
    fifo_create(&fifo, 16);
    
    assert(fifo_empty(fifo) == true);

    for (int i = 0, status = 0; status != -1; i++)
        status = fifo_push(&fifo, i);
    
    assert(fifo_full(fifo) == true);

    fifo_print(fifo);

    fifo_peek(fifo);
    fifo_pop(&fifo);
    fifo_pop(&fifo);
    fifo_pop(&fifo);

    fifo_print(fifo);

    assert(fifo_full(fifo) == false);
    fifo_destroy(&fifo);
}

void LLtest(void) {
    ll_node_t *list = NULL;
    
    // expected: 1 2 3
    ll_prepend(&list, 0XBFC00000);
    ll_append(&list, 3);
    ll_insert(&list, 2, 1);
    ll_print(list);
    
    // expected: 1
    printf("%d\n", ll_find(list, 4));

    // expected: 
    ll_remove_find(&list, 2);
    ll_remove_first(&list);
    ll_remove_last(&list);
    ll_remove_index(&list, 0);
    ll_print(list);

    // expected: list is inaccessable
    ll_destroy(list);
}

void BSTtest() {
    bt_node_t *root = NULL;
    int data[7] = {10, 11, 9, 3, 21, 15, 7};
    for (int i = 0; i < 7; i++)
        root = BSTinsert(root, data[i]);
    BSTdisplay(root, 0);
    BSTdelete(root);
}

void DarrayTest(void) {
    darray_t *da = darray_create(5);

    darray_insert(da, "Hello");
    darray_insert(da, "World");
    darray_insert(da, "Goodbye");
    darray_insert(da, "Space");
    
    int length = da->length;
    const char **contents = darray_contents(da);
    
    for (int i = 0; i < length; i++)
        printf("%s\n", contents[i]);

    while (da->length > 0)
        printf("%s\n", darray_remove(da));

    darray_destroy(da);
}

int main() {
    BSTtest();
    LLtest();
    FIFOtest();
    DarrayTest();
}
