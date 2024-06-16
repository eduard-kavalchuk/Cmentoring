#ifndef BST_H
#define BST_H

typedef struct BinaryTreeNode *node_t;

void bst_insertNode(node_t *root, int value);
void bst_print(node_t root);
int bst_findMin(node_t root, int *x);
int bst_findMax(node_t root, int *x);
void bst_delete(node_t *root, int x);
void bst_clear(node_t *root);
void bst_toArray(node_t root, int *array, int size);
int bst_getSize(node_t root);

#endif