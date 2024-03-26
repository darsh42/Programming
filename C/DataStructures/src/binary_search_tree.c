#include "datastructures.h"
#define COUNT 10

Node *node_create(int value) {
    Node *node = malloc(sizeof(Node));
    node->value = value;
    node->left  = NULL;
    node->right = NULL;
    return node;
}

void BSTdelete(Node *root) {
    if (root->left != NULL) {
        BSTdelete(root->left);
    }
    if (root->right != NULL) {
        BSTdelete(root->right);
    }
    free(root);
}

Node *BSTinsert(Node *root, int value) {
    if (root == NULL) {
        return node_create(value);
    } else if (value < root->value) {
        root->left = BSTinsert(root->left, value);
    } else if (value > root->value) {
        root->right = BSTinsert(root->right, value);
    }
    return root;
}

void BSTremove(Node *root, int value) {}

void BSTsearch(Node *root, Node *result, int value) {}

// Function to print binary tree in 2D
// It does reverse inorder traversal
void BSTdisplay(struct Node* root, int space) {
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    BSTdisplay(root->right, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d\n", root->value);

    // Process left child
    BSTdisplay(root->left, space);
}

