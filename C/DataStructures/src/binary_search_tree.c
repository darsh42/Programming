#include "common.h"
#define COUNT 10

typedef struct BINARY_TREE_NODE {
    int value;
    struct BINARY_TREE_NODE *left;
    struct BINARY_TREE_NODE *right;
} bt_node_t;

bt_node_t *node_create(int value) {
    bt_node_t *node = malloc(sizeof(bt_node_t));
    node->value = value;
    node->left  = NULL;
    node->right = NULL;
    return node;
}

void BSTdelete(bt_node_t *root) {
    if (root->left != NULL) {
        BSTdelete(root->left);
    }
    if (root->right != NULL) {
        BSTdelete(root->right);
    }
    free(root);
}

bt_node_t *BSTinsert(bt_node_t *root, int value) {
    if (root == NULL) {
        return node_create(value);
    } else if (value < root->value) {
        root->left = BSTinsert(root->left, value);
    } else if (value > root->value) {
        root->right = BSTinsert(root->right, value);
    }
    return root;
}

void BSTremove(bt_node_t *root, int value) {}

void BSTsearch(bt_node_t *root, bt_node_t *result, int value) {}

// Function to print binary tree in 2D
// It does reverse inorder traversal
void BSTdisplay(bt_node_t* root, int space) {
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

