#include "datastructures.h"

void BSTtest() {
    Node *root = NULL;
    int data[7] = {10, 11, 9, 3, 21, 15, 7};
    for (int i = 0; i < 7; i++)
        root = BSTinsert(root, data[i]);
    BSTdisplay(root, 0);
    BSTdelete(root);
}

int main() {
    BSTtest();
}
