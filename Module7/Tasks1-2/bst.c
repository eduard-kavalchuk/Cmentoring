#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

static void bst_toArray_(node_t root, int *array, int size, int *count);
static node_t newNodeCreate(int value);
static void bst_getSize_(node_t root, int *count);

struct BinaryTreeNode {
	int key;
	struct BinaryTreeNode *left, *right;
};

static node_t newNodeCreate(int value)
{
	node_t temp = (node_t)malloc(sizeof(struct BinaryTreeNode));
	temp->key = value;
	temp->left = temp->right = NULL;
	return temp;
}

void bst_insertNode(node_t *node, int value)
{
	if (*node == NULL) {
		*node = newNodeCreate(value);
	}
	if (value < (*node)->key) {
		bst_insertNode(&(*node)->left, value);
	}
	else if (value > (*node)->key) {
		bst_insertNode(&(*node)->right, value);
	}
}

void bst_print(node_t root)
{
	if (root == NULL) return;
    
    bst_print(root->left);
    printf("%d ", root->key);
    bst_print(root->right);
}

int bst_findMin(node_t root, int *storage)
{
	if (root == NULL) {
		return -1;
	}
	else if (root->left != NULL) {
		return bst_findMin(root->left, storage);
	}
    *storage = root->key;
	return 0;
}

int bst_findMax(node_t root, int *storage)
{
	if (root == NULL) {
		return -1;
	}
	else if (root->right != NULL) {
		return bst_findMax(root->right, storage);
	}
    *storage = root->key;
	return 0;
}

void bst_delete(node_t *root, int x)
{
	if (*root == NULL)
		return;

	if (x > (*root)->key) {
		bst_delete(&(*root)->right, x);
	}
	else if (x < (*root)->key) {
		bst_delete(&(*root)->left, x);
	}
	else {
		if ((*root)->left == NULL && (*root)->right == NULL) {
			free(*root);
			*root = NULL;
		}
		else if ((*root)->left == NULL || (*root)->right == NULL) {
			node_t temp;
			if ((*root)->left == NULL) {
				temp = (*root)->right;
			}
			else {
				temp = (*root)->left;
			}
			free(*root);
			*root = temp;
		}
		else {
            bst_findMin((*root)->right, &(*root)->key);
			bst_delete(&(*root)->right, (*root)->key);
		}
	}
}

void bst_clear(node_t *root)
{
    if (*root == NULL) return;
    
    bst_clear(&(*root)->left);
    bst_clear(&(*root)->right);
    free(*root);
    *root = NULL;
}

void bst_toArray(node_t root, int *array, int size)
{
    int count = 0;
    
    if (root == NULL) return;
    
    bst_toArray_(root, array, size, &count);
}

static void bst_toArray_(node_t root, int *array, int size, int *count)
{    
    if (root != NULL) {
        bst_toArray_(root->left, array, size, count);
        array[(*count)++] = root->key;
        bst_toArray_(root->right, array, size, count);
    }
}

int bst_getSize(node_t root)
{
    int count = 0;
    
    if (root == NULL) return 0;
    
    bst_getSize_(root, &count);
    return count;
}

static void bst_getSize_(node_t root, int *count)
{
    if (root != NULL) {
        bst_getSize_(root->left, count);
        (*count)++;
        bst_getSize_(root->right, count);
    }
}
