#ifndef BST_H
#define BST_H

typedef struct BinaryTreeNode *node_t;

node_t newNodeCreate(int value);
node_t insertNode(node_t node, int value);
void inOrder(node_t root);
void *findMin(node_t root);
void *findMax(node_t root);
node_t delete(node_t root, int x);
void clear(node_t *root);
void traverse(node_t root, int *array, int size);

#endif