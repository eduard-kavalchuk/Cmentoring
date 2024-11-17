#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <math.h>

#define NUM_CHARS 127
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define GET_BIT(value, pos) ((((value) >> (pos)) & 1) == 1 ? 1 : 0)
#define BYTESIZE(sizeInBits) ((sizeInBits) / 8 + MIN((sizeInBits) % 8, 1))
#define BIT_COUNT(value) (value != 0 ? (int)log2(value) + 1 : 0)

typedef struct Node Node;

struct Node {
    int weight;
    int character;
    uint16_t bin;
    uint8_t binSize;
    Node *childNodes[2];
};


void insertNode(Node* element);
Node* createInternalNode(Node *node1, Node *node2);
int getParentIdx(int childIdx);
int getLeftChildIdx(int parentIdx);
int getRightChildIdx(int parentIdx);
void heapify(int startFromIndex);
Node* pop(void);
void printCurrentLevel(Node **root, int idx, int level);
void printLevelOrder(Node **root);
void binarize(void);
void _binarize(uint16_t bin, uint8_t binSize, Node *node);
void flatten(Node *node);
unsigned char* serialize(char *string, int strSize, int *bufSize);
Node* getNodeByChar(char c);


static char *inputStr = "preved medved";
Node* heap[2 * NUM_CHARS - 1];
int size;

int main(void)
{
    char inputArray[NUM_CHARS] = {0};
    
    for (int i = 0; i < strlen(inputStr); i++) {
        inputArray[(int) inputStr[i]] += 1;
    }
    
    for (int i = 0; i < NUM_CHARS + 1; i++) {
        if (0 == inputArray[i]) continue;
        
        Node *node = (Node *) calloc(1, sizeof(Node));
        node->weight = inputArray[i];
        node->character = i;
        insertNode(node);
    }

    printLevelOrder(heap);
    
    while(size != 1) {
        Node* node = createInternalNode(pop(), pop());
        insertNode(node);
        //printf("===================================\n");
        //printLevelOrder(heap);
    }
    
    binarize();
    flatten(pop());
    
    unsigned char* compressedString;
    int sizeInBits;
    compressedString = serialize(inputStr, strlen(inputStr), &sizeInBits);
    
    printf("sizeInBits = %d\n", sizeInBits);
    for(int i = 0; i < BYTESIZE(sizeInBits); i++) {
        printf("compressedString[%d] = %d\n", i, compressedString[i]);
    }
    
    
    
    return 0;
}


unsigned char* serialize(char *inputString, int strSize, int *bitSize)
{
    int shift = 7, bufferCursor = 0, sizeInBits = 0;
    unsigned char *buffer = (unsigned char *) calloc(1, strSize * sizeof(int));
    
    for(int i = 0; i < strSize; i++) {
        Node* node = getNodeByChar(inputString[i]);
        for(int j = node->binSize - 1; j >= 0 ; j--) {
            if(1 == GET_BIT(node->bin, j)) {
                buffer[bufferCursor] |= (1 << shift);
            }
            if(--shift < 0) {
                shift = 7;
                bufferCursor++;
            }
            ++sizeInBits;
        }
    }
    
    buffer = realloc(buffer, BYTESIZE(sizeInBits));
    *bitSize = sizeInBits;
    
    return buffer;
}

Node* getNodeByChar(char c)
{
    for(int i = 0; i < size; i++) {
        if(heap[i]->character == c) {
            return heap[i];
        }
    }
    return NULL;
}

void flatten(Node *node)
{
    if (NULL == node) return;
    
    if ((NULL == node->childNodes[0]) && (NULL == node->childNodes[1])) {      
        printf("(%c, %d, %d)\n", node->character, node->bin, node->binSize);
        heap[size++] = node;
    }
    
    flatten(node->childNodes[0]);
    flatten(node->childNodes[1]);
}


void _binarize(uint16_t bin, uint8_t binSize, Node *node) {
    if (node == NULL) return;
    
    if ((NULL == node->childNodes[0]) && (NULL == node->childNodes[1])) {
        node->bin = bin;
        node->binSize = binSize;
    }
    
    if(0 == binSize) {
        _binarize(0, 1, node->childNodes[0]);
        _binarize(1, 1, node->childNodes[1]);
    }
    else {
        _binarize(bin << 1, binSize + 1, node->childNodes[0]);
        _binarize((bin << 1) + 1, binSize + 1, node->childNodes[1]);
    }
}

void binarize(void)
{
    _binarize(0, 0, heap[0]);
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

Node* createInternalNode(Node *node1, Node *node2)
{
    Node *node = (Node *) calloc(1, sizeof(Node));
    node->weight = node1->weight + node2->weight;
    node->character = NUM_CHARS + 1;
    node->childNodes[0] = node1;
    node->childNodes[1] = node2;

    return node;
}

Node* pop(void)
{
    Node *ret = heap[0];
    
    heap[0] = heap[size - 1];
    size--;
    heapify(0);
    
    return ret;
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
}

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






