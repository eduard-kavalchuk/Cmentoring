/*
    TODO
    Уйти от типов Node2 и Node3, оставить только Node.
*/

#include "MinHeap.h"

static char *str = "preved medved";
char output[128];
int output_ptr;


typedef struct Node Node;

struct Node {
    int weight;
    char character;
    char *repr;
    Node *left;
    Node *right;
};


void fill_array(char *str, char *input)
{
    for (int i = 0; i < strlen(str); i++) {
        input[(unsigned char)str[i]] += 1;
    }
}


int comparator(void *e1, void *e2)
{
    int w1 = ((Node *) e1)->weight, w2 = ((Node *) e2)->weight;
    if (w1 > w2) return 1;
    else if (w1 == w2) return 0;
    else return -1;
}

void setWeight(void *e, int weight)
{
    ((Node *) e)->weight = weight;
}

int getWeight(void *e)
{
    return ((Node *) e)->weight;
}


void weight_setter(void *e, int weight)
{
    ((Node *) e)->weight = weight;
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

Node* join(Node *node1, Node *node2)
{
    Node *node = (Node *) calloc(1, sizeof(Node));
    node->weight = node1->weight + node2->weight;
    node->character = 255;
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
        output[output_ptr++] = root->character;
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

typedef struct Node2 Node2;

struct Node2 {
    char character;
    char *str;
    Node2 *left;
    Node2 *right;
};

Node2 array[128];
int node2Cnt = 0;

void assign_binary_code(char *str, Node *node) {
    if (node == NULL) return;
    
    if (node->character != (char) 255) {
        Node2 *node2 = (Node2 *) calloc(1, sizeof(Node2));
        node2->character = node->character;
        
        char *s = (char *) malloc(strlen(str));
        strcpy(s, str);
        node2->str = s;
        
        
        array[node2Cnt] = *node2;
        ++node2Cnt;
        printf("node2Cnt = %d, node->weight = %d, node->character = %d, node->left = %p, node->right = %p\n", node2Cnt, node->weight, node->character, node->left, node->right);
    }
    
    char *s1 = (char *) malloc(strlen(str) + 1);
    strcpy(s1, str);
    strcat(s1, "0");
    assign_binary_code(s1, node->left);
    
    char *s2 = (char *) malloc(strlen(str) + 1);
    strcpy(s2, str);
    strcat(s2, "1");
    assign_binary_code(s2, node->right);
    
}


typedef struct Node3 Node3;

struct Node3 {
    char c;
    char *str;
};


int compare_by_char(const void* a, const void* b) {
    // If a is smaller, positive value will be returned
    return ((Node3 *)a)->c - ((Node3 *)b)->c;
}


int compare_by_binary(const void* a, const void* b) {
    // If a is smaller, positive value will be returned
    char *str1 = ((Node3 *)a)->str;
    char *str2 = ((Node3 *)b)->str;
    
    return strcmp(str1, str2);
}


int find_code(char c, Node3 *array, int len)
{
    int position = 0, left = 0, right = len - 1;
    while (left <= right) {
        int middle = left + (right - left) / 2;
        if (array[middle].c >= c) {
            right = middle - 1;
            position = middle;
        } else {
            left = middle + 1;
        }
    }
    if (array[position].c != c)
        return -1;
    return position;
}


char find_key(char *key, Node3 *array, int len)
{
    int position = 0, left = 0, right = len - 1;
    while (left <= right) {
        int middle = left + (right - left) / 2;
        int cmp = strcmp(array[middle].str, key);
        if (cmp >= 0) {
            right = middle - 1;
            position = middle;
        } else {
            left = middle + 1;
        }
    }
    if (strcmp(array[position].str, key) != 0)
        return -1;
    return array[position].c;
}




int main() {
    // Create minimal heap with a capacity of 100 elements
    MinHeap* heap = init_minheap(100, comparator, setWeight, getWeight);
    
    // Read input string and assign weights to input characters
    char input[128] = {0};
    fill_array(str, input);
    
    // Build min heap with input characters ordered by weights
    for (int i = 0; i < 127; i++) {
        if (!input[i]) continue;
        
        Node *node = (Node *) calloc(1, sizeof(Node));
        node->weight = input[i];
        node->character = i;
        insert_minheap(heap, node);
    }
    
    print_heap(heap);
    
    // Keep popping two min elements, joining them into one and inserting into the heap
    // until only one element is left
    Node* arr[100] = {};
    int s = 0;
    
    while(heap->size > 1) {
        arr[s++] = delete_element(heap, 0);
        arr[s++] = delete_element(heap, 0);
        insert_minheap(heap, join(arr[s - 2], arr[s - 1]));
        print_heap(heap);
    }

    
    Node *node = delete_element(heap, 0);
    
    // Walk the tree and assign binary code to each leaf (i. e. input character) and
    // produce and array of (input_character, binary_code) elements
    char *st = "";
    assign_binary_code(st, node); 
    
    printf("\n");
    for (int i = 0; i < node2Cnt; i++) {
        printf("(%c, %s) ", array[i].character, array[i].str);
    }
    
    
    // Build array of (input_character, binary_code) elements stripped off all other info
    Node3 arr2[node2Cnt];
    
    for (int i = 0; i < node2Cnt; i++) {
        Node3 *node = (Node3 *) calloc(1, sizeof(Node3));
        char *s = (char *) malloc(strlen(array[i].str));
        strcpy(s, array[i].str);
        node->c = array[i].character;
        node->str = s;
        arr2[i] = *node;
    }
    
    printf("\n");
    for (int i = 0; i < node2Cnt; i++) {
        printf("(%c, %s) ", arr2[i].c, arr2[i].str);
    }
    
    
    // Sort resulting array using qsort
    int n = sizeof(arr2) / sizeof(arr2[0]);
    qsort(arr2, n, sizeof(Node3), compare_by_char);
    
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("(%c, %s) -> ", arr2[i].c, arr2[i].str);
    }
    
    
    // Encode string (convert input string to binary code)
    printf("\n");
    char *s3 = (char *) malloc(100);
    for (int i = 0 ; i < strlen(str); i++) {
        int v = find_code(str[i], arr2, node2Cnt);
        printf("(%c, %s) ", str[i], arr2[v].str);
        strcat(s3, arr2[v].str);
    }
    
    qsort(arr2, n, sizeof(Node3), compare_by_binary);
    
    printf("\nBinary output:\n%s\n", s3);

    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("(%c, %s) -> ", arr2[i].c, arr2[i].str);
    }
    
    
    // Decode string
    // 1110111110110100100000110011101001
    
    char *decoded_string = (char *) calloc(1, 100);
    char *tmp = (char *) calloc(1, 100);
    int total_decoded = 0;
    int start = 0, end = 1;
    char c;
    
    while(end <= strlen(s3)) {
        strncpy(tmp, s3 + start, end - start);
        if ((c = find_key(tmp, arr2, n)) != -1) {
            decoded_string[total_decoded++] = c;
            start = end;
            memset(tmp, '\0', strlen(tmp));
        }
        ++end;
        
    }
    
    printf("\nDecoded string = %s\n", decoded_string);
    
    
    return 0;
}

