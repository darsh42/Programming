#include "linked_list.h"

int add_itemSingle(LinkList *L, int val) {
    LinkList *l;
    for (l = L; l != NULL; l = l->next)
        ;

    l = malloc(sizeof(LinkList));

    l->next = NULL;
    l->data = val;
    return 0;
}

int del_itemSingle(LinkList *L, int val) {
    LinkList *l;
    for (l = L; l->data != val && l != NULL; l = l->next)
        ;

    for (LinkList *tmp = NULL; l->next != NULL; l = tmp) {
        l->data = l->next->data;
        tmp = l->next->next;
    }
}
