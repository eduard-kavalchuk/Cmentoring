#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>

#define NUM_CHARS 127
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define GET_BIT(value, pos) ((((value) >> (pos)) & 1) == 1 ? 1 : 0)
#define BYTESIZE(sizeOfContentInBits) ((sizeOfContentInBits) / 8 + MIN((sizeOfContentInBits) % 8, 1))
#define BIT_COUNT(value) (value != 0 ? (int)log2(value) + 1 : 0)

typedef struct Node Node;

struct Node {
    int weight;
    int character;
    uint16_t bin;
    uint8_t binSize;
    Node *childNodes[2];
};

typedef struct encodingHeaderItem encodingHeaderItem;
    
struct encodingHeaderItem {
    uint16_t bin;
    uint16_t binSize;
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
void assignCodesToChars(void);
void _assignCodesToChars(uint16_t bin, uint8_t binSize, Node *node);
void flatten(Node *node);
int serialize(char *inputString, void *encodingTable, unsigned char **compressedContent);
int getCharRepr(char c, void *encodingTable, uint16_t *repr, uint8_t *reprSize);
int getNextBit(unsigned char *binary, int binSize);
int findChar(uint8_t value, uint8_t len, void *encodingTable);

//static char *inputStr = "preved medved";
Node* heap[2 * NUM_CHARS - 1];
int size;
void registerChar(char ch, int weight, uint16_t bin, uint8_t binSize);
void* buildEncodingTable(void);
int buildEncodingHeader(encodingHeaderItem **encodingHeader, void *encodingTable);
void buildOutputBinary(unsigned char** archive, unsigned char* compressedContent, int sizeOfContentInBits, encodingHeaderItem* encodingHeader, int sizeOfEncodingHeader, int sizeOfLengthHeader);
int restoreContentLength(unsigned char* archive);
void* restoreEncodingTable(unsigned char* archive);
uint8_t* restoreTextContent(unsigned char* archive, int contentLengthInBits, void *decodedEncodingTable);
uint8_t* extractBinaryContent(unsigned char* archive, int contentLengthInBits);
void compress(char *filename);
void extract(char *filename);
long getFileSize(char *filename);


int main(int argc, char *argv[])
{
    int opt;
    void (*func)(char *filename);
    
    while ((opt = getopt(argc, argv, "c:x:")) != -1) {
        switch (opt) {
            case 'c':
                func = compress;
                break;
            case 'x':
                func = extract;
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s [-c file] [-x file]\n", argv[0]);
                return -1;
        }
    }
   
    if((optind != 3) && (argc != 3)) {
        fprintf(stderr, "Usage: %s [-c filename] [-x filename]\n", argv[0]);
        return -1;
    }
    
    func(argv[2]);
    

    return 0;
}


long getFileSize(char *filename)
{
    FILE *fp = fopen(filename, "rb"); 
    if (fp == NULL) {
        return -1; // Error opening file
    }

    fseek(fp, 0L, SEEK_END); // Move file pointer to the end
    long filesize = ftell(fp); // Get the current position (file size)
    fclose(fp);
    
    return filesize;
}


void extract(char *filename)
{    
    long filesize = getFileSize(filename);
    if (filesize == -1) {
        exit(0);
    }
    
    uint8_t *archive = (uint8_t *) calloc(1, filesize);
    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        exit(0);
    }
    
    fread(archive, sizeof(uint8_t), filesize, fp);
    fclose(fp);
    
    // Step 1. Decode length of content
    int contentLengthInBits = restoreContentLength(archive);
    printf("Length of compressed content in bits = %d\n", contentLengthInBits);
    
    // Step 2. Decode Huffman encoding
    void *decodedEncodingTable = restoreEncodingTable(archive);
    
    for(int i = 0; i < size ; i++) {
        printf("(%c, %d, %d)\n", heap[i]->character, heap[i]->bin, heap[i]->binSize);
    }
    
    // Step 3. Decode original string
    uint8_t *restoredBinContent = extractBinaryContent(archive, contentLengthInBits);
    printf("\nsize = %d\n", size);
    uint8_t *content = restoreTextContent(restoredBinContent, contentLengthInBits, decodedEncodingTable);
    printf("%s\n", content);
}


void compress(char *filename)
{
    //char *inputStr = "preved medve\r\n123\r\nsdrt\nw45";
    
    long sz =  getFileSize(filename);
    
    char *inputStr = (char *) calloc(1, sz + 1);
    FILE *fp;
    
    fp = fopen(filename, "rb"); 
    if (fp == NULL) {
        return; // Error opening file
    }
    fread(inputStr, sizeof(char), sz + 1, fp);
    fclose(fp);
    
    printf("%s\n", inputStr);
    
    char charFrequencies[NUM_CHARS] = {0};
    
    for (int i = 0; i < strlen(inputStr); i++) {
        charFrequencies[(int) inputStr[i]] += 1;
    }
    
    for (int ch = 0; ch < NUM_CHARS + 1; ch++) {
        if (0 == charFrequencies[ch]) continue;

        registerChar(ch, charFrequencies[ch], 0, 0);
    }
    
    void *encodingTable = buildEncodingTable();

    unsigned char* compressedContent;
    int sizeOfContentInBits = serialize(inputStr, encodingTable, &compressedContent);
    
    printf("\nCompressed binary content\n");
    for(int i = 0; i < BYTESIZE(sizeOfContentInBits); i++) {
        printf("%d ", compressedContent[i]);
    }
    printf("End\n");
    
    printf("sizeOfContentInBits = %d\n", sizeOfContentInBits);
    /*
    for(int i = 0; i < BYTESIZE(sizeOfContentInBits); i++) {
        printf("compressedContent[%d] = %d\n", i, compressedContent[i]);
    }
    */
    
    printf("\nEncoding phase...\n");
    
    /**************************************
      Encode
    **************************************/
    
    encodingHeaderItem* encodingHeader; 
    int sizeOfEncodingHeader = buildEncodingHeader(&encodingHeader, encodingTable);
    
    unsigned char* archive;
    int sizeOfLengthHeader = sizeof(int);
    
    buildOutputBinary(&archive, compressedContent, sizeOfContentInBits, encodingHeader, sizeOfEncodingHeader, sizeOfLengthHeader);
    
    fp = fopen(strcat(filename, ".huf"), "wb"); 
    if (fp == NULL) {
        return; // Error opening file
    }
    
    int totalsize = sizeOfLengthHeader + sizeOfEncodingHeader + BYTESIZE(sizeOfContentInBits);
    printf("total size = %d\n", totalsize);
    fwrite(archive, sizeof(uint8_t), totalsize, fp);
    fclose(fp);
}


uint8_t* extractBinaryContent(unsigned char* archive, int contentLengthInBits)
{
    uint8_t *restoredBinaryContent = (uint8_t *) calloc(1, BYTESIZE(contentLengthInBits));
    
    memcpy(restoredBinaryContent, archive + sizeof(int) + NUM_CHARS * sizeof(encodingHeaderItem), BYTESIZE(contentLengthInBits));
    
    
    
    return restoredBinaryContent;
}


uint8_t* restoreTextContent(unsigned char* binContent, int contentLengthInBits, void *encodingTable)
{
    /*
    printf("\nRestored binary content:\n");
    for(int i = 0; i < BYTESIZE(contentLengthInBits); i++) {
        printf("%d ", binContent[i]);
    }
    printf("End.\n");
    */
    
    uint8_t *restoredTextContent = (uint8_t *) calloc(1, contentLengthInBits + 1);
    int totalCharsRestored = 0, binCode = 0, codeLen = 0;
    
    for(int i = 0; i < contentLengthInBits; i++) {
        uint8_t bit = getNextBit(binContent, contentLengthInBits);
        binCode = (binCode << 1) + bit;
        ++codeLen;
        char c = findChar(binCode, codeLen, encodingTable);
        if (c != -1) {
            restoredTextContent[totalCharsRestored++] = c;
            binCode = 0;
            codeLen = 0;
        }
    }
    
    restoredTextContent = realloc(restoredTextContent, totalCharsRestored);
    
    return restoredTextContent;
}


void* restoreEncodingTable(unsigned char* archive)
{
    size = 0;
    for(int i = 0; i < NUM_CHARS; i++) {
        encodingHeaderItem item;
        memcpy(&item, archive + sizeof(int) + i * sizeof(encodingHeaderItem), sizeof(encodingHeaderItem));
        if(0 != item.binSize) {
            registerChar((unsigned char) i, 0, item.bin, item.binSize);
        }
    }
    return heap;
}


int restoreContentLength(unsigned char* archive)
{   
    int len;
    
    memcpy(&len, archive, sizeof(int));
    
    return len;
}


void buildOutputBinary(unsigned char** archive, unsigned char* compressedContent, int sizeOfContentInBits, encodingHeaderItem* encodingHeader, int sizeOfEncodingHeader, int sizeOfLengthHeader)
{        
    unsigned char* out = (unsigned char*) calloc(1, sizeOfLengthHeader + sizeOfEncodingHeader + BYTESIZE(sizeOfContentInBits));
    
    memcpy(out, &sizeOfContentInBits, sizeOfLengthHeader);    
    memcpy(out + sizeOfLengthHeader, encodingHeader, sizeOfEncodingHeader);
    memcpy(out + sizeOfLengthHeader + sizeOfEncodingHeader, compressedContent, BYTESIZE(sizeOfContentInBits));
    
    *archive = out;
}


int buildEncodingHeader(encodingHeaderItem **encodingHeader, void *encodingTable)
{
    Node **tree = (Node **) encodingTable;
    int sizeOfHeader = NUM_CHARS * sizeof(encodingHeaderItem);
    encodingHeaderItem* header = (encodingHeaderItem *) calloc(1, sizeOfHeader); 
    for(int i = 0; i < size; i++) {
        encodingHeaderItem item;
        item.bin = tree[i]->bin;
        item.binSize = tree[i]->binSize;
        header[tree[i]->character] = item;
    }
    *encodingHeader = header;
    
    return sizeOfHeader;
}


int findChar(uint8_t value, uint8_t len, void *encodingTable)
{
    Node **tree = (Node **) encodingTable;
    
    for(int i = 0; i < size ; i++) {
        if((tree[i]->bin == value) && (tree[i]->binSize == len))
            return tree[i]->character;
    }
    
    return -1;
}


void* buildEncodingTable(void)
{
    while(size != 1) {
        Node* node = createInternalNode(pop(), pop());
        insertNode(node);
    }
    
    assignCodesToChars();
    flatten(pop());
    
    return heap;
}

void registerChar(char ch, int weight, uint16_t bin, uint8_t binSize)
{
    Node *node = (Node *) calloc(1, sizeof(Node));
    node->weight = weight;
    node->character = ch;
    node->bin = bin;
    node->binSize = binSize;
    insertNode(node);
}


int getNextBit(unsigned char *binary, int bitSize)
{
    static int cursor = 0;
    
    for(; cursor < bitSize; cursor++) {
        uint8_t byte = binary[cursor / 8];
        int bit = GET_BIT(byte, 7 - cursor % 8);
        ++cursor;
        return bit;
    }
    return -1;
}

int serialize(char *inputString, void *encodingTable, unsigned char **compressedContent)
{
    int shift = 7, bufferCursor = 0, sizeOfContentInBits = 0;
    unsigned char *buffer = (unsigned char *) calloc(1, strlen(inputString) * sizeof(int));
    
    for(int i = 0; i < strlen(inputString); i++) {
        uint16_t repr;
        uint8_t reprSize; 
        int status = getCharRepr(inputString[i], encodingTable, &repr, &reprSize);
        
        if(status == -1) {
            printf("status == -1\n");
        }
        for(int j = reprSize - 1; j >= 0 ; j--) {
            if(1 == GET_BIT(repr, j)) {
                buffer[bufferCursor] |= (1 << shift);
            }
            --shift;
            if(shift < 0) {
                shift = 7;
                bufferCursor++;
            }
            ++sizeOfContentInBits;
        }
        //printf("(%c, %d, %d)\n", inputString[i], repr, reprSize);
    }
    
    *compressedContent = realloc(buffer, BYTESIZE(sizeOfContentInBits));
    
    return sizeOfContentInBits;
}

int getCharRepr(char c, void *encodingTable, uint16_t *repr, uint8_t *reprSize)
{
    Node **tree = (Node **)encodingTable;
    
    for(int i = 0; i < size; i++) {
        if(tree[i]->character == c) {
            *repr = tree[i]->bin;
            *reprSize = tree[i]->binSize;
            return 0;
        }
    }
    return -1;
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


void _assignCodesToChars(uint16_t bin, uint8_t binSize, Node *node) {
    if (node == NULL) return;
    
    if ((NULL == node->childNodes[0]) && (NULL == node->childNodes[1])) {
        node->bin = bin;
        node->binSize = binSize;
    }
    
    if(0 == binSize) {
        _assignCodesToChars(0, 1, node->childNodes[0]);
        _assignCodesToChars(1, 1, node->childNodes[1]);
    }
    else {
        _assignCodesToChars(bin << 1, binSize + 1, node->childNodes[0]);
        _assignCodesToChars((bin << 1) + 1, binSize + 1, node->childNodes[1]);
    }
}

void assignCodesToChars(void)
{
    _assignCodesToChars(0, 0, heap[0]);
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


int height(void)
{
    return (int)log2(size) + 1;
}


void printLevelOrder(Node **root) {
    int h = height();
    
    for (int i = 1; i <= h; i++) {
        printCurrentLevel(root, 0, i);
        printf("\n");
    }
}

