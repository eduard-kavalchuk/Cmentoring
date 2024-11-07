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
int getNextBit(unsigned char *binary, int binSize);
int findChar(uint8_t value, uint8_t len);

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
    
    
    printf("\nEncoding phase....\n");
    
    // Encode
    typedef struct encodingHeaderItem encodingHeaderItem;
    
    struct encodingHeaderItem {
        uint16_t bin : 12;
        uint16_t binSize : 4;
    };
    
    encodingHeaderItem* encodingHeader = (encodingHeaderItem *) calloc(NUM_CHARS, sizeof(encodingHeaderItem));
    
    for(int i = 0; i < size; i++) {
        encodingHeaderItem item;
        item.bin = heap[i]->bin;
        item.binSize = heap[i]->binSize;
        encodingHeader[heap[i]->character] = item;
    }
    
    int sizeOfLengthHeader = sizeof(int);
    int sizeOfEncodingHeader = NUM_CHARS * sizeof(encodingHeaderItem);
    int sizeOfContentInBytes = BYTESIZE(sizeInBits);
    
    unsigned char* outputBinary = (unsigned char*) calloc(1, sizeOfLengthHeader + sizeOfEncodingHeader + sizeOfContentInBytes);
    
    memcpy(outputBinary, &sizeInBits, sizeOfLengthHeader);    
    memcpy(outputBinary + sizeOfLengthHeader, encodingHeader, sizeOfEncodingHeader);
    memcpy(outputBinary + sizeOfLengthHeader + sizeOfEncodingHeader, compressedString, sizeOfContentInBytes);
    
    
    
    
    printf("\nDecoding phase....\n");
    
    // Decode
    size = 0;
    
    // Step 1. Decode length of content
    int outputLength;
    memcpy(&outputLength, outputBinary, sizeOfLengthHeader);
    printf("Length of compressed content in bits = %d\n", outputLength);
    
    // Step 2. Decode Huffman encoding
    size = 0;
    for(int i = 0; i < NUM_CHARS; i++) {
        encodingHeaderItem item;
        memcpy(&item, outputBinary + sizeOfLengthHeader + i * sizeof(encodingHeaderItem), sizeof(encodingHeaderItem));
        if(0 != item.binSize) {
            Node *node = (Node *) calloc(1, sizeof(Node));
            node->character = (unsigned char) i;
            node->bin = item.bin;
            node->binSize = item.binSize;
            heap[size++] = node;
        }
    }
    
    for(int i = 0; i < size ; i++) {
        printf("(%c, %d, %d)\n", heap[i]->character, heap[i]->bin, heap[i]->binSize);
    }
    
    // Step 3. Decode original string
    uint8_t *binary = (uint8_t *) calloc(1, BYTESIZE(outputLength));
    memcpy(binary, outputBinary + sizeOfLengthHeader + NUM_CHARS * sizeof(encodingHeaderItem), BYTESIZE(outputLength));
    
    
    uint8_t value = 0, len = 0;
    for(int i = 0; i < outputLength; i++) {
        uint8_t bit = getNextBit(binary, outputLength);
        value = (value << 1) + bit;
        ++len;
        int c = findChar(value, len);
        if (c != -1) {
            printf("%c", c);
            value = 0;
            len = 0;
        }
    }

    
    return 0;
}


int findChar(uint8_t value, uint8_t len)
{
    for(int i = 0; i < size ; i++) {
        if((heap[i]->bin == value) && (heap[i]->binSize == len))
            return heap[i]->character;
    }
    
    return -1;
}


int getNextBit(unsigned char *binary, int bitSize)
{
    static unsigned char cursor = 0;
    
    for(; cursor < bitSize; cursor++) {
        uint8_t byte = binary[cursor / 8];
        int bit = GET_BIT(byte, 7 - cursor % 8);
        //printf("byte = %d, bit = %d, cursor = %d\n", byte, bit, cursor);
        ++cursor;
        return bit;
    }
    return -1;
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






