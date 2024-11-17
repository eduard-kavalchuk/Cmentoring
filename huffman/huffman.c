#include "huffman.h"


int main(int argc, char *argv[])
{
    int opt;
    void (*func)(char *inFile, char *outputBinary);
    
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
                exit(EXIT_FAILURE);
        }
    }
   
    if((optind != 4) && (argc != 4)) {
        fprintf(stderr, "Usage: %s [-c filename] [-x filename]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    func(argv[2], argv[3]);
    
    freeAll();
    
    exit(EXIT_SUCCESS);
}


long getFileSize(char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (NULL == fp) {
        fprintf(stderr, "Error opening file %s\n", filename);
        freeAll();
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0L, SEEK_END);
    long filesize = ftell(fp);
    fclose(fp);
    
    return filesize;
}


void compress(char *inFile, char *outputBinary)
{
    long numBytes =  getFileSize(inFile);
    if(0 == numBytes) {
        fprintf(stderr, "File %s has 0 length.\n", inFile);
        return;
    }
    
    uint8_t *inputBytes;
    
    if((inputBytes = (uint8_t *) calloc(1, numBytes + 1))) {
        gc[gcCounter++] = (void *) inputBytes;
    }
    else {
        fprintf(stderr, "Failed to allocate memory in line %d\n", __LINE__);
        freeAll();
        exit(EXIT_FAILURE);
    }
    
    FILE *fp = fopen(inFile, "rb"); 
    if (NULL == fp) {
        fprintf(stderr, "Error opening file %s\n", inFile);
        freeAll();
        exit(EXIT_FAILURE);
    }
    
    fread(inputBytes, sizeof(uint8_t), numBytes + 1, fp);
    fclose(fp);
    
    int charFrequencies[NUM_CHARS] = {0};
    
    for (int i = 0; i < numBytes; i++) {
        charFrequencies[inputBytes[i]] += 1;
    }
    
    for (uint16_t ch = 0; ch < NUM_CHARS; ch++) {
        if (0 == charFrequencies[ch]) {
            continue;
        }

        registerChar((uint8_t)ch, charFrequencies[ch], 0, 0);
    }
    
    Node **encodingTable = buildEncodingTable();

    uint8_t* compressedContent;
    int sizeOfContentInBits = serialize(inputBytes, numBytes, encodingTable, &compressedContent);

    encodingSegmentItem* encodingSegment; 
    int sizeOfEncodingSegment = buildEncodingSegment(&encodingSegment, encodingTable);
    int sizeOfLengthSegment = sizeof(int);
    int imageSize = sizeOfLengthSegment + sizeOfEncodingSegment + BYTESIZE(sizeOfContentInBits);
    
    uint8_t* image;
    image = createImage(compressedContent, sizeOfContentInBits, encodingSegment, sizeOfEncodingSegment, sizeOfLengthSegment);
    
    fp = fopen(outputBinary, "wb"); 
    if (NULL == fp) {
        fprintf(stderr, "Error opening file %s\n", outputBinary);
        freeAll();
        exit(EXIT_FAILURE);
    }
    
    fwrite(image, sizeof(uint8_t), imageSize, fp);
    fclose(fp);
}


void extract(char *inFile, char *outputBinary)
{    
    long filesize = getFileSize(inFile);
    if (0 == filesize) {
        fprintf(stderr, "File %s has 0 length.\n", inFile);
        return;
    }
    
    uint8_t *archive = (uint8_t *) calloc(1, filesize);
    if(NULL != archive) {
        gc[gcCounter++] = (void *) archive;
    }
    else {
        fprintf(stderr, "Failed to allocate memory in line %d\n", __LINE__);
        freeAll();
        exit(EXIT_FAILURE);
    }
    
    FILE *fp = fopen(inFile, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file %s\n", inFile);
        freeAll();
        exit(EXIT_FAILURE);
    }
    
    fread(archive, sizeof(uint8_t), filesize, fp);
    fclose(fp);
    
    int contentLengthInBits = restoreContentLength(archive);
    
    Node **encodingTable = restoreEncodingTable(archive);

    uint8_t *restoredBinContent = stripHeader(archive, contentLengthInBits);
    int restoredSize = 0;
    uint8_t *content = restoreContent(restoredBinContent, contentLengthInBits, encodingTable, &restoredSize);
    
    fp = fopen(outputBinary, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file %s\n", inFile);
        freeAll();
        exit(EXIT_FAILURE);
    }

    fwrite(content, sizeof(uint8_t), restoredSize, fp);
    fclose(fp);
}


uint8_t* stripHeader(uint8_t* binInput, int contentLengthInBits)
{
    uint8_t *restoredBinaryContent = (uint8_t *) calloc(1, BYTESIZE(contentLengthInBits));
    if(restoredBinaryContent != NULL) {
        gc[gcCounter++] = (void *) restoredBinaryContent;
    }  
    else {
        fprintf(stderr, "Failed to allocate memory in line %d\n", __LINE__);
        freeAll();
        exit(EXIT_FAILURE);
    }

    memcpy(restoredBinaryContent, binInput + sizeof(int) + NUM_CHARS * sizeof(encodingSegmentItem), BYTESIZE(contentLengthInBits));
    
    return restoredBinaryContent;
}


uint8_t* restoreContent(uint8_t* binContent, int contentLengthInBits, Node **encodingTable, int *restoredSize)
{   
    uint8_t *content = (uint8_t *) calloc(1, contentLengthInBits + 1);    
    if(content == NULL) {
        fprintf(stderr, "Failed to allocate memory in line %d\n", __LINE__);
        freeAll();
        exit(EXIT_FAILURE);
    }
    
    int totalBytesRestored = 0;
    uint16_t binCode = 0, codeLen = 0;
    
    for(int i = 0; i < contentLengthInBits; i++) {
        int bit = getNextBit(binContent, contentLengthInBits);
        if(-1 == bit) {
            fprintf(stderr, "Failed to restore content.\n");
            freeAll();
            exit(EXIT_FAILURE);
        }
        binCode = (binCode << 1) + bit;
        ++codeLen;
        if(codeLen > 15) {
            fprintf(stderr, "Failed to restore content.\n");
            freeAll();
            exit(EXIT_FAILURE);
        }
        
        int c = findChar(binCode, codeLen, encodingTable);
        if (c != -1) {
            content[totalBytesRestored++] = (uint8_t) c;
            binCode = 0;
            codeLen = 0;
        }
    }
    
    if ((content = (uint8_t *) realloc(content, totalBytesRestored))) {
        *restoredSize = totalBytesRestored;
        gc[gcCounter++] = (void *) content;
    }
    else {
        fprintf(stderr, "Failed to re-allocate memory in line %d\n", __LINE__);
        freeAll();
        exit(EXIT_FAILURE);
    }
    
    return content;
}


Node** restoreEncodingTable(uint8_t* archive)
{
    size = 0;
    for(int i = 0; i < NUM_CHARS; i++) {
        encodingSegmentItem item;
        memcpy(&item, archive + sizeof(int) + i * sizeof(encodingSegmentItem), sizeof(encodingSegmentItem));
        if(0 != item.binSize) {
            registerChar((uint8_t) i, 0, item.bin, item.binSize);
        }
    }
    
    return Heap;
}


int restoreContentLength(uint8_t* archive)
{   
    int len;
    memcpy(&len, archive, sizeof(int));
    
    return len;
}


uint8_t* createImage(uint8_t* compressedContent, int sizeOfContentInBits, encodingSegmentItem* encodingSegment, int sizeOfEncodingSegment, int sizeOfLengthSegment)
{        
    uint8_t* outputBin = (uint8_t*) calloc(1, sizeOfLengthSegment + sizeOfEncodingSegment + BYTESIZE(sizeOfContentInBits));
    if(NULL == outputBin) {
        fprintf(stderr, "Failed to allocate memory in line %d\n", __LINE__);
        freeAll();
        exit(EXIT_FAILURE);
    }
    else {
        gc[gcCounter++] = (void *) outputBin;
    }
    
    memcpy(outputBin, &sizeOfContentInBits, sizeOfLengthSegment);    
    memcpy(outputBin + sizeOfLengthSegment, encodingSegment, sizeOfEncodingSegment);
    memcpy(outputBin + sizeOfLengthSegment + sizeOfEncodingSegment, compressedContent, BYTESIZE(sizeOfContentInBits));
    
    return outputBin;
}


int buildEncodingSegment(encodingSegmentItem **encodingSegment, Node **encodingTable)
{
    int sizeOfEncodingSegment = NUM_CHARS * sizeof(encodingSegmentItem);
    encodingSegmentItem* segment = (encodingSegmentItem *) calloc(1, sizeOfEncodingSegment);
    if(NULL != segment) {
        gc[gcCounter++] = (void *) segment;
    }
    else {
        fprintf(stderr, "Failed to allocate memory in line %d\n", __LINE__);
        freeAll();
        exit(EXIT_FAILURE);
    }
    
    for(int i = 0; i < size; i++) {
        encodingSegmentItem item;
        item.bin = encodingTable[i]->bin;
        item.binSize = encodingTable[i]->binSize;
        segment[encodingTable[i]->character] = item;
    }
    
    *encodingSegment = segment;
    
    return sizeOfEncodingSegment;
}


int findChar(uint16_t value, uint16_t len, Node **encodingTable)
{    
    for(int i = 0; i < size ; i++) {
        if((encodingTable[i]->bin == value) && (encodingTable[i]->binSize == len))
            return encodingTable[i]->character;
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
    
    return Heap;
}

void registerChar(uint8_t ch, int weight, uint16_t bin, uint8_t binSize)
{
    Node *node = (Node *) calloc(1, sizeof(Node));
    if(NULL != node) {
        gc[gcCounter++] = (void *) node;
    }
    else {
        fprintf(stderr, "Failed to allocate memory in line %d\n", __LINE__);
        freeAll();
        exit(EXIT_FAILURE);
    }
    
    node->weight = weight;
    node->character = ch;
    node->bin = bin;
    node->binSize = binSize;
    insertNode(node);
}


int getNextBit(uint8_t *binary, int bitSize)
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

int serialize(uint8_t *inputBytes, int numBytes, Node **encodingTable, uint8_t **compressedContent)
{
    int shift = 7, bufferCursor = 0, sizeOfContentInBits = 0;
    uint8_t *buffer = (uint8_t *) calloc(1, numBytes * sizeof(int));
    if(NULL == buffer) {
        fprintf(stderr, "Failed to re-allocate memory in line %d\n", __LINE__);
        freeAll();
        exit(EXIT_FAILURE);
    }
    
    for(int i = 0; i < numBytes; i++) {
        uint16_t repr;
        uint8_t reprSize;
        
        int status = getCharRepr(inputBytes[i], encodingTable, &repr, &reprSize);
        if(status == -1) {
            fprintf(stderr, "Failed to compress file.\n");
            exit(EXIT_FAILURE);
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
    }
    
    if((*compressedContent = (uint8_t *) realloc(buffer, BYTESIZE(sizeOfContentInBits)))) {
        gc[gcCounter++] = (void *) *compressedContent;
    }
    else
    {
        fprintf(stderr, "Failed to re-allocate memory in line %d\n", __LINE__);
        freeAll();
        exit(EXIT_FAILURE);
    }
    
    return sizeOfContentInBits;
}

int getCharRepr(uint8_t c, Node **encodingTable, uint16_t *repr, uint8_t *reprSize)
{    
    for(int i = 0; i < size; i++) {
        if(encodingTable[i]->character == c) {
            *repr = encodingTable[i]->bin;
            *reprSize = encodingTable[i]->binSize;
            return 0;
        }
    }
    return -1;
}

void flatten(Node *node)
{
    if (NULL == node) return;
    
    if ((NULL == node->childNodes[0]) && (NULL == node->childNodes[1])) {
        Heap[size++] = node;
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
    _assignCodesToChars(0, 0, Heap[0]);
}

void insertNode(Node* element) {
    int curr = size;
    
    Heap[size++] = element;
    while (curr > 0 && Heap[getParentIdx(curr)]->weight > Heap[curr]->weight) {
        Node *temp = Heap[getParentIdx(curr)];
        Heap[getParentIdx(curr)] = Heap[curr];
        Heap[curr] = temp;
        curr = getParentIdx(curr);
    }
}

Node* createInternalNode(Node *node1, Node *node2)
{
    Node *node = (Node *) calloc(1, sizeof(Node));
    if(NULL != node) {
        gc[gcCounter++] = (void *) node;
    }
    else {
        fprintf(stderr, "Failed to allocate memory in line %d\n", __LINE__);
        freeAll();
        exit(EXIT_FAILURE);
    }
    
    node->weight = node1->weight + node2->weight;
    node->character = NUM_CHARS + 1;
    node->childNodes[0] = node1;
    node->childNodes[1] = node2;

    return node;
}

Node* pop(void)
{
    Node *ret = Heap[0];
    
    Heap[0] = Heap[size - 1];
    size--;
    rebuildHeap(0);
    
    return ret;
}

void rebuildHeap(int index) {
    if (size <= 1)
        return;
    
    int left = getLeftChildIdx(index); 
    int right = getRightChildIdx(index); 
    
    int smallest = index; 
    
    if (left < size && Heap[left]->weight < Heap[index]->weight)
        smallest = left; 
    
    if (right < size && Heap[right]->weight  < Heap[smallest]->weight) 
        smallest = right; 

    if (smallest != index) 
    { 
        Node *temp = Heap[index];
        Heap[index] = Heap[smallest];
        Heap[smallest] = temp;
        rebuildHeap(smallest); 
    }
}

void freeAll(void)
{
    for(int i = 0; i < gcCounter; i++) {
        free(gc[i]);
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
            printf("(%d, %d)  ", root[idx]->character, root[idx]->weight);
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
