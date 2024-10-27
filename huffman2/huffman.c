#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LEAF 0
#define INTERNAL 1
#define NUM_CHARS 127

typedef struct Node Node;

struct Node {
    int weight;
    int character;
    char *repr;
    int nodeType;
    Node *nextLevelNodes[2];
};

static char *inputStr = "preved medved";
char inputArray[2 * NUM_CHARS - 1];
Node* heap[2 * NUM_CHARS - 1];
int size = 0;
Node* garbage[NUM_CHARS - 1];
int garbageTracker;


int getParentIdx(int i) {
    return (i - 1) / 2;
}

int getLeftChildIdx(int i) {
    return (2 * i + 1);
}

int getRightChildIdx(int i) {
    return (2 * i + 2);
}

int height(void)
{
    return (int)log2(size) + 1;
}

void printCurrentLevel(Node **root, int idx, int level) {
    if (idx >= size)
        return;
    if (level == 1) {
        if (root[idx]->character == ' ')
            printf("('', %d)  ", root[idx]->weight);
        else if(root[idx]->character > NUM_CHARS)
            printf("(%d, %d)  ", root[idx]->character, root[idx]->weight);
        else
            printf("(%c, %d)  ", root[idx]->character, root[idx]->weight);
    }
    else if (level > 1) {
        printCurrentLevel(root, getLeftChildIdx(idx), level - 1);
        printCurrentLevel(root, getRightChildIdx(idx), level - 1);
    }
}

void printLevelOrder(Node **root) {
    int h = height();
    
    for (int i = 1; i <= h; i++) {
        printCurrentLevel(root, 0, i);
        printf("\n");
    }
}

void printNode(Node *node)
{
    if (node->character < NUM_CHARS)
        printf("(%c, %d) ", node->character, node->weight);
    else
        printf("(%d, %d) ", node->character, node->weight);
}


void insertNode(Node* element) {
    int curr = size;
    
    heap[size++] = element;
    while (curr > 0 && heap[getParentIdx(curr)]->weight > heap[curr]->weight) {
        Node *temp = heap[getParentIdx(curr)];
        heap[getParentIdx(curr)] = heap[curr];
        heap[curr] = temp;
        curr = getParentIdx(curr);
    }
}

void heapify(int index) {
    if (size <= 1)
        return;
    
    int left = getLeftChildIdx(index); 
    int right = getRightChildIdx(index); 
    
    int smallest = index; 
    
    if (left < size && heap[left]->weight < heap[index]->weight )
        smallest = left; 
    
    if (right < size && heap[right]->weight  < heap[smallest]->weight ) 
        smallest = right; 

    if (smallest != index) 
    { 
        Node *temp = heap[index];
        heap[index] = heap[smallest];
        heap[smallest] = temp;
        heapify(smallest); 
    }

    return;
}


Node* pop()
{
    Node *ret = heap[0];
    
    heap[0] = heap[size - 1];
    size--;
    heapify(0);
    
    return ret;
}

void do_map(char *str, Node *node) {
    if (node == NULL) return;
    
    if (LEAF == node->nodeType) {
        node->repr = (char *) malloc(strlen(str));
        strcpy(node->repr, str);
    }
    
    char *s1 = (char *) malloc(strlen(str) + 1);
    strcpy(s1, str);
    strcat(s1, "0");
    do_map(s1, node->nextLevelNodes[0]);
    
    char *s2 = (char *) malloc(strlen(str) + 1);
    strcpy(s2, str);
    strcat(s2, "1");
    do_map(s2, node->nextLevelNodes[1]); 
}


void mapSymbolsToCodes()
{
    do_map("", heap[0]);
}


Node* createInternalNode(Node *node1, Node *node2)
{
    Node *node = (Node *) calloc(1, sizeof(Node));
    node->weight = node1->weight + node2->weight;
    node->character = NUM_CHARS + 1;
    node->nextLevelNodes[0] = node1;
    node->nextLevelNodes[1] = node2;
    node->nodeType = INTERNAL;

    return node;
}


void do_create_dict(Node *node)
{
    if (node == NULL) return;
    
    if (LEAF == node->nodeType) {      
        printf("\n(%c, %s)", node->character, node->repr);
        heap[size++] = node;
    }
    else {
        garbage[garbageTracker++] = node;
    }
    
    do_create_dict(node->nextLevelNodes[0]);
    do_create_dict(node->nextLevelNodes[1]);
}

int compare_by_char(const void* a, const void* b) {
    Node *one = *(Node **)a;
    Node *two = *(Node **)b;
    return ((Node *)one)->character - ((Node *)two)->character;
}

int compare_by_binary(const void* a, const void* b) {   
    Node *one = *(Node **)a;
    Node *two = *(Node **)b;
    return strcmp(((Node *)one)->repr, ((Node *)two)->repr);
}

void createHuffmanDict()
{
    do_create_dict(pop());
    qsort(heap, size, sizeof(Node*), compare_by_char);
}


char* encodeChar(char c)
{
    int position = 0, left = 0, right = size - 1;
    while (left <= right) {
        int middle = left + (right - left) / 2;
        if (heap[middle]->character >= c) {
            right = middle - 1;
            position = middle;
        } else {
            left = middle + 1;
        }
    }
    
    if (heap[position]->character != c)
        return NULL;
    else
        return heap[position]->repr;
}

char findCharByEncoding(char *binaryCode)
{
    int position = 0, left = 0, right = size - 1;
    while (left <= right) {
        int middle = left + (right - left) / 2;
        int cmp = strcmp(heap[middle]->repr, binaryCode);
        if (cmp >= 0) {
            right = middle - 1;
            position = middle;
        } else {
            left = middle + 1;
        }
    }
    
    if (strcmp(heap[position]->repr, binaryCode) != 0)
        return -1;
    else
        return heap[position]->character;
}

void stringToHeap(char *inputStr)
{
    for (int i = 0; i < strlen(inputStr); i++) {
        inputArray[(int) inputStr[i]] += 1;
    }
    
    for (int i = 0; i < NUM_CHARS + 1; i++) {
        if (!inputArray[i]) continue;
        
        Node *node = (Node *) calloc(1, sizeof(Node));
        node->weight = inputArray[i];
        node->character = i;
        node->nodeType = LEAF;
        insertNode(node);
    }
}

void createHuffmanTreeFromHeap()
{
    while(size != 1) {
        Node* node = createInternalNode(pop(), pop());
        insertNode(node);
        printf("===================================\n");
        printLevelOrder(heap);
    }
}

char* encode(char* inputStr)
{
    char *encodedStr = (char *) malloc(8 * strlen(inputStr));
    for (int i = 0 ; i < strlen(inputStr); i++) {
        strcat(encodedStr, encodeChar(inputStr[i]));
    }
    
    return encodedStr;
}


char* decode(char *encodedStr)
{
    char *decoded_string = (char *) calloc(1, 100);
    char *tmp = (char *) calloc(1, 100);
    int total_decoded = 0;
    int start = 0, end = 1;
    char c;
    
    qsort(heap, size, sizeof(Node*), compare_by_binary);
    
    printf("\n");
    for (int i = 0; i < size; i++) {
        printf("(%c, %s) -> ", heap[i]->character, heap[i]->repr);
    }
    
    while(end <= strlen(encodedStr)) {
        strncpy(tmp, encodedStr + start, end - start);
        if ((c = findCharByEncoding(tmp)) != -1) {
            decoded_string[total_decoded++] = c;
            start = end;
            memset(tmp, '\0', strlen(tmp));
        }
        ++end;
    }
    
    return decoded_string;
}

void cleanup()
{
    for(int i = 0; i < garbageTracker; i++) {
        free(garbage[i]);
    }
    for(int i = 0; i < size; i++) {
        free(heap[i]->repr);
        free(heap[i]);
    }
}


int main()
{
    stringToHeap(inputStr);
    
    printf("Initial heap:\n");
    printLevelOrder(heap);

    createHuffmanTreeFromHeap();
    mapSymbolsToCodes();
    
    createHuffmanDict();
    
    printf("\n");
    for (int i = 0; i < size; i++) {
        printf("(%c, %s) ", heap[i]->character, heap[i]->repr);
    }
    
    char *encodedStr = encode(inputStr);
    
    printf("\nBinary output:\n%s\n", encodedStr);
    
    char* decoded_string;
    decoded_string = decode(encodedStr);
    printf("\nDecoded string = %s\n", decoded_string);
    
    cleanup();

    return 0;
}
