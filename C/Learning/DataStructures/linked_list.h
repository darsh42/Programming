#ifndef LINKED_LIST_H_INCLUDED
#define LINKED_LIST_H_INCLUDED

#include "common.h"

typedef struct LinkedList {
    struct LinkedList *next;
    struct LinkedList *prev;
    int data;
} LinkList;

#endif // LINKED_LIST_H_INCLUDED
