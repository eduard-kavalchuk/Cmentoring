#ifndef BST_H
#define BST_H

typedef struct BinaryTreeNode *node_t;

node_t newNodeCreate(int value);
node_t searchNode(node_t root, int target);
node_t insertNode(node_t node, int value);
void postOrder(node_t root);
void inOrder(node_t root);
void preOrder(node_t root);
void *findMin(node_t root);
void *findMax(node_t root);
node_t delete(node_t root, int x);
void clear(node_t *root);

#endif