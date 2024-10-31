#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <math.h>

#define LEAF 0
#define INTERNAL 1
#define NUM_CHARS 127
#define MIN(a, b) (((a) < (b)) ? (a) : (b))


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
        printf("(%c, %s) ", node->character, node->repr);
    else
        printf("(%d, %s) ", node->character, node->repr);
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
    
    char *encodedStr = (char *) malloc(8 * strlen(inputStr));
    for (int i = 0 ; i < strlen(inputStr); i++) {
        strcat(encodedStr, encodeChar(inputStr[i]));
    }
    
    return encodedStr;
}


char* decode(char *encodedStr)
{
    char *decodedString = (char *) calloc(1, 100);
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
            decodedString[total_decoded++] = c;
            start = end;
            memset(tmp, '\0', strlen(tmp));
        }
        ++end;
    }
    
    return decodedString;
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

unsigned char* encodeBinary(char *str)
{    
    unsigned char *buffer = (unsigned char *) calloc(1, strlen(str) / 8 + MIN(1, strlen(str) % 8));
    int shift = 7, bufferCursor = 0;
    
    for(int strCursor = 0; strCursor < strlen(str); strCursor++) {
        if(str[strCursor] == '1') {
            buffer[bufferCursor] |= (1 << shift);
        }
        if(--shift < 0) {
            shift = 7;
            bufferCursor++;
        }
    }
    
    return buffer;
}


uint16_t* encodeRepr(char *str)
{    
    uint16_t *buffer = (uint16_t *) calloc(1, sizeof(uint16_t));
    int shift = strlen(str) - 1, bufferCursor = 0;
    
    for(int strCursor = 0; strCursor < strlen(str); strCursor++) {
        if(str[strCursor] == '1') {
            buffer[bufferCursor] |= (1 << shift);
        }
        --shift;
    }
    
    return buffer;
}


//#define BIT(byte, k) (((byte) >> (k)) & 1)
#define GET_BIT(byte, k) (((((byte) >> (k)) & 1)) == 1 ? '1' : '0')
#define LENGTH_IN_BYTES(bitBuffer) (strlen(bitBuffer) / 8 + MIN(1, strlen(bitBuffer) % 8))

char* binToString(int encoded, int size) {
    printf("encoded = %d, size = %d\n", encoded, size);
    char *binToStr = (char *) calloc(1, size + 1); // sizeof(encodedStr) is a bit length of binary buffer
    for(int i = 0; i < size; i++) {
        binToStr[size - i - 1] = GET_BIT(encoded, i);
    }
    return binToStr;
}


int main()
{
    char *encodedStr = encode(inputStr);
    
    printf("\nBinary output:\n%s\n", encodedStr);
    
    for(int i = 0; i < size; i++) {
        printNode(heap[i]);
    }
    
    /*
    char* decodedString;
    decodedString = decode(encodedStr);
    printf("\nDecoded string = %s\n", decodedString);
    */
    
    
    // 00010000 01001011 01111100 11000101 10
    unsigned char *content = encodeBinary(encodedStr);
    for(int i = 0; i < LENGTH_IN_BYTES(encodedStr); i++) {
        printf("\nByte[%d] = %d", i, content[i]);
    }
    
    // Decode content
    char *binToStr = (char *) calloc(1, sizeof(encodedStr) + 1); // sizeof(encodedStr) is a bit length of binary buffer
    for(int i = 0; i < strlen(encodedStr); i++) {
        binToStr[i] = GET_BIT(content[i / 8], 7 - i % 8);
    }
    
    printf("\nbinToStr:\n%s\n", binToStr);
    
    // Block containing content's length
    int lenBlock = (int)strlen(encodedStr);
    unsigned int* b = (unsigned int*) calloc(1, sizeof(int));
    memcpy(b, &lenBlock, sizeof(int));

    // Block containing header
    struct pack {
        uint16_t size : 4;
        uint16_t content : 12;
    };
    
    struct pack* header = (struct pack *) calloc(127, sizeof(struct pack));
    for(int i = 0; i < size; i++) {
        struct pack sp;
        char *repr = heap[i]->repr;
        uint16_t *binRepr = (uint16_t *) encodeRepr(repr);
        sp.size = strlen(repr);
        sp.content = *binRepr;
        header[heap[i]->character] = sp;
    }
    
    int size1 = sizeof(int), size2 = 127 * sizeof(struct pack), size3 = LENGTH_IN_BYTES(encodedStr);
    unsigned char* outputBinary = (unsigned char*) calloc(1, size1 + size2 + size3);
    memcpy(outputBinary, b, size1);    
    memcpy(outputBinary + size1, header, size2);
    memcpy(outputBinary + size1 + size2, encodedStr, size3);
    
    int outputLength;
    memcpy(&outputLength, outputBinary, size1);
    printf("length = %d\n", *outputBinary);
    
    cleanup();
    
    size = 0;
    for(int i = 0; i < 127; i++) {
        struct pack sp;
        memcpy(&sp, outputBinary + size1 + i * sizeof(struct pack), sizeof(struct pack));
        if(sp.size) {
            Node *node = (Node *) calloc(1, sizeof(Node));
            node->character = (unsigned char) i;
            
            
            int content = sp.content;
            content &= (int) pow(2, sp.size) - 1;
            //node->repr = binToString(content, sp.size);
            node->weight = content;
            //printf("sp.size = %d, 2 ^ sp.size = %d\n", sp.size, (int) pow(2, sp.size));
            
            
            node->repr = binToString(content, sp.size);
            
            /*
                TODO:
                1. Наложить на sp.content маску размером sp.size
                2. Результат преобразовать в строку с нулями и единицами
            */
            insertNode(node);
        }
    }
    
    for(int i = 0; i < size ; i++) {
        printf("character = %c, weight = %d, repr = %s\n", heap[i]->character, heap[i]->weight, heap[i]->repr);
    }
    
    
    
    
    /*
    for(int i = 0; i < 127; i++) {
        printf("size = %d, repr = %d\n", header[i].size, header[i].content);
    }
    */
    
    
    

    
    /*
    struct Node {
        int weight;
        int character;
        char *repr;
        int nodeType;
        Node *nextLevelNodes[2];
    };
    
    */
    
    
    //cleanup();

    return 0;
}
