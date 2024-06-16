#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

static void traverse_(node_t root, int *array, int size, int *count);

struct BinaryTreeNode {
	int key;
	struct BinaryTreeNode *left, *right;
};

// Function to create a new node with a given value
node_t newNodeCreate(int value)
{
	node_t temp = (node_t)malloc(sizeof(struct BinaryTreeNode));
	temp->key = value;
	temp->left = temp->right = NULL;
	return temp;
}

// Function to insert a node with a specific value in the
// tree
node_t insertNode(node_t node, int value)
{
	if (node == NULL) {
		return newNodeCreate(value);
	}
	if (value < node->key) {
		node->left = insertNode(node->left, value);
	}
	else if (value > node->key) {
		node->right = insertNode(node->right, value);
	}
	return node;
}

// Function to perform in-order traversal
void inOrder(node_t root)
{
	if (root != NULL) {
		inOrder(root->left);
		printf("%d ", root->key);
		inOrder(root->right);
	}
}

// Function to find the minimum value
int findMin(node_t root, int *storage)
{
	if (root == NULL) {
		return -1;
	}
	else if (root->left != NULL) {
		return findMin(root->left, storage);
	}
    *storage = root->key;
	return 0;
}

// Function to find the maximum value
int findMax(node_t root, int *storage)
{
	if (root == NULL) {
		return -1;
	}
	else if (root->right != NULL) {
		return findMax(root->right, storage);
	}
    *storage = root->key;
	return 0;
}

// Function to delete a node from the tree
node_t delete(node_t root, int x)
{
	if (root == NULL)
		return NULL;

	if (x > root->key) {
		root->right = delete (root->right, x);
	}
	else if (x < root->key) {
		root->left = delete (root->left, x);
	}
	else {
		if (root->left == NULL && root->right == NULL) {
			free(root);
			return NULL;
		}
		else if (root->left == NULL
				|| root->right == NULL) {
			node_t temp;
			if (root->left == NULL) {
				temp = root->right;
			}
			else {
				temp = root->left;
			}
			free(root);
			return temp;
		}
		else {
            findMin(root->right, &root->key);
			root->right = delete(root->right, root->key);
		}
	}
	return root;
}

void clear(node_t *root)
{
    if (*root != NULL) {
		clear(&(*root)->left);
		clear(&(*root)->right);
        free(*root);
        *root = NULL;
	}
}

void traverse(node_t root, int *array, int size)
{
    int count = 0;
    traverse_(root, array, size, &count);
}

static void traverse_(node_t root, int *array, int size, int *count)
{
    if (root != NULL) {
		traverse_(root->left, array, size, count);
        array[(*count)++] = root->key;
		traverse_(root->right, array, size, count);
	}
}
