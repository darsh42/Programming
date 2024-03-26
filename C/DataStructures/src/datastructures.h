#ifndef DATASTRUCTURES_H_INCLUDED
#define DATASTRUCTURES_H_INCLUDED

#include "common.h"

typedef struct Node Node;
struct Node {
    int value;
    Node *left;
    Node *right;
};

Node *BSTinsert(Node *root, int value);
void BSTremove(Node *root, int value);
void BSTdelete(Node *root);
void BSTsearch(Node *root, Node *result, int value);
void BSTdisplay(Node *root, int space);

#endif // DATASTRUCTURES_H_INCLUDED
