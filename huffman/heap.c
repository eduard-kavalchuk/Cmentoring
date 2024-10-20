#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"


static char *str = "preved medved";
char input[255];

MinHeap* heapify(MinHeap* heap, int index);

struct Node {
    int weight;
    unsigned char character;
    char *repr;
    int isLeaf;
    Node *left;
    Node *right;
};

void fill_array(char *str)
{
    for (int i = 0; i < strlen(str); i++) {
        input[(unsigned char)str[i]] += 1;
    }
}

void setWeight(MinHeap *heap, void *e, int weight)
{
    ((Node *) e)->weight = weight;
    heapify(heap, 0);
}

int getWeight(void *e)
{
    return ((Node *) e)->weight;
}


int compare(void *e1, void *e2)
{
    int w1 = ((Node *) e1)->weight, w2 = ((Node *) e2)->weight;
    if (w1 > w2) return 1;
    else if (w1 == w2) return 0;
    else return -1;
}

void print_heap(MinHeap* heap) {
    // Simply print the array. This is an
    // inorder traversal of the tree
    printf("Min Heap:\n");
    for (int i=0; i<heap->size; i++) {
        printf("(%d, %c)-> ", ((Node *)(heap->arr[i]))->weight, ((Node *)(heap->arr[i]))->character);
    }
    printf("\n");
}


void free_minheap(MinHeap* heap) {
    if (!heap)
        return;
    free(heap->arr);
    free(heap);
}

Node* join(Node *node1, Node *node2)
{
    Node *node = (Node *) calloc(1, sizeof(Node));
    node->weight = node1->weight + node2->weight;
    node->isLeaf = 0;
    node->left = node1;
    node->right = node2;
    
    return node;
}

// Compute the "height" of a tree -- 
int height(Node *node) {
    if (node == NULL)
        return 0;
    else {
        int lheight = height(node->left);
        int rheight = height(node->right);
        if (lheight > rheight)
            return (lheight + 1);
        else
            return (rheight + 1);
    }
}

// Print nodes at a current level
void printCurrentLevel(Node *root, int level) {
    if (root == NULL)
        return;
    if (level == 1) {
        printf("(%d, %c) -> ", root->weight, root->character);
    }
    else if (level > 1) {
        printCurrentLevel(root->left, level - 1);
        printCurrentLevel(root->right, level - 1);
    }
}

void printLevelOrder(Node *root) {
    int h = height(root);
    
    for (int i = 1; i <= h; i++)
        printCurrentLevel(root, i);
}


void make(char *str, Node *node) {
    if (node == NULL) return;
    
    if (node->isLeaf) {
        node->repr = (char *) malloc(strlen(str));
        strcpy(node->repr, str);
    }
    
    char *s1 = (char *) malloc(strlen(str) + 1);
    strcpy(s1, str);
    strcat(s1, "0");
    make(s1, node->left);
    
    char *s2 = (char *) malloc(strlen(str) + 1);
    strcpy(s2, str);
    strcat(s2, "1");
    make(s2, node->right);
}

void flatten(Node *node, Node *array, int *node2Cnt)
{
    if (node == NULL) return;
    
    if (node->isLeaf) {      
        printf("\n(%c, %s)", node->character, node->repr);
        array[(*node2Cnt)++] = *node;
    }
    
    flatten(node->left, array, node2Cnt);
    flatten(node->right, array, node2Cnt);
}


int compare_by_char(const void* a, const void* b) {
    return ((Node *)a)->character - ((Node *)b)->character;
}


int compare_by_binary(const void* a, const void* b) {   
    return strcmp(((Node *)a)->repr, ((Node *)b)->repr);
}


int find_code(char c, Node *array, int len)
{
    int position = 0, left = 0, right = len - 1;
    while (left <= right) {
        int middle = left + (right - left) / 2;
        if (array[middle].character >= c) {
            right = middle - 1;
            position = middle;
        } else {
            left = middle + 1;
        }
    }
    if (array[position].character != c)
        return -1;
    return position;
}


char find_key(char *key, Node *array, int len)
{
    int position = 0, left = 0, right = len - 1;
    while (left <= right) {
        int middle = left + (right - left) / 2;
        int cmp = strcmp(array[middle].repr, key);
        if (cmp >= 0) {
            right = middle - 1;
            position = middle;
        } else {
            left = middle + 1;
        }
    }
    if (strcmp(array[position].repr, key) != 0)
        return -1;
    return array[position].character;
}




int main() {
    // Capacity of 100 elements
    MinHeap* heap = init_minheap(100, compare, setWeight, getWeight);
    
    fill_array(str);
    
    for (int i = 0; i < 254; i++) {
        if (!input[i]) continue;
        
        Node *node = (Node *) calloc(1, sizeof(Node));
        node->weight = input[i];
        node->character = i;
        node->isLeaf = 1;
        insert_minheap(heap, node);
    }
    
    printf("Heap before folding:\n");
    print_heap(heap);
    
    while(heap->size > 1) {
        Node *node1 = (Node *) calloc(1, sizeof(Node));
        memcpy(node1, peek(heap, 0), sizeof(Node));
        delete_element(heap, 0);
        
        Node *node2 = (Node *) calloc(1, sizeof(Node));
        memcpy(node2, peek(heap, 0), sizeof(Node));
        delete_element(heap, 0);
        
        insert_minheap(heap, join(node1, node2));
        
        print_heap(heap);
    }

    
    Node *node = (Node *) peek(heap, 0);
    
    make("", node); 
    
    Node array[255];
    int node2Cnt = 0;
    
    flatten(node, array, &node2Cnt);
    printf("\nnode2Cnt = %d", node2Cnt);
    
    
    printf("\n");
    for (int i = 0; i < node2Cnt; i++) {
        printf("(%c, %s) ", array[i].character, array[i].repr);
    }
    
    // Sort the array using qsort
    qsort(array, node2Cnt, sizeof(Node), compare_by_char);
    
    printf("\n");
    for (int i = 0; i < node2Cnt; i++) {
        printf("(%c, %s) -> ", array[i].character, array[i].repr);
    }
    
    
    // Convert input string to binary code
    printf("\n");
    char *s3 = (char *) malloc(100);
    for (int i = 0 ; i < strlen(str); i++) {
        int v = find_code(str[i], array, node2Cnt);
        strcat(s3, array[v].repr);
    }
    
    printf("\nBinary output:\n%s\n", s3);
    
    
    qsort(array, node2Cnt, sizeof(Node), compare_by_binary);
    
    printf("\n");
    for (int i = 0; i < node2Cnt; i++) {
        printf("(%c, %s) -> ", array[i].character, array[i].repr);
    }
    
    
    // decode

    // 1110111110110100100000110011101001
    
    char *decoded_string = (char *) calloc(1, 100);
    char *tmp = (char *) calloc(1, 100);
    int total_decoded = 0;
    int start = 0, end = 1;
    char c;
    
    while(end <= strlen(s3)) {
        strncpy(tmp, s3 + start, end - start);
        if ((c = find_key(tmp, array, node2Cnt)) != -1) {
            decoded_string[total_decoded++] = c;
            start = end;
            memset(tmp, '\0', strlen(tmp));
        }
        ++end;
        
    }
    
    printf("\nDecoded string = %s\n", decoded_string);
    
    
    return 0;
}

