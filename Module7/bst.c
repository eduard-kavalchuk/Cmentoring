#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

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

// Function to search for a node with a specific key in the
// tree
node_t searchNode(node_t root, int target)
{
	if (root == NULL || root->key == target) {
		return root;
	}
	if (root->key < target) {
		return searchNode(root->right, target);
	}
	return searchNode(root->left, target);
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
void *findMin(node_t root)
{
	if (root == NULL) {
		return NULL;
	}
	else if (root->left != NULL) {
		return findMin(root->left);
	}
	return &root->key;
}

// Function to find the maximum value
void *findMax(node_t root)
{
	if (root == NULL) {
		return NULL;
	}
	else if (root->right != NULL) {
		return findMax(root->right);
	}
	return &root->key;
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
			node_t temp = findMin(root->right);
			root->key = temp->key;
			root->right = delete (root->right, temp->key);
		}
	}
	return root;
}

void clear(node_t *root)
{
    if (*root != NULL) {
		clear(&(*root)->left);
		clear(&(*root)->right);
        printf("Deleting %d\n", (*root)->key);
        free(*root);
        *root = NULL;
	}
}
