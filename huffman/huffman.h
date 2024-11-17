#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>

#define NUM_CHARS 256
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define GET_BIT(value, pos) ((((value) >> (pos)) & 1) == 1 ? 1 : 0)
#define BYTESIZE(sizeOfContentInBits) ((sizeOfContentInBits) / 8 + MIN((sizeOfContentInBits) % 8, 1))
#define BIT_COUNT(value) (value != 0 ? (int)log2(value) + 1 : 0)
#define MAX_HEAP_SIZE (2 * NUM_CHARS - 1)

typedef struct Node Node;

struct Node {
    int weight;
    int character;
    uint16_t bin;
    uint8_t binSize;
    Node *childNodes[2];
};

typedef struct encodingSegmentItem encodingSegmentItem;
    
struct encodingSegmentItem {
    uint16_t bin;
    uint16_t binSize;
};


void insertNode(Node* element);
Node* createInternalNode(Node *node1, Node *node2);
int getParentIdx(int childIdx);
int getLeftChildIdx(int parentIdx);
int getRightChildIdx(int parentIdx);
void rebuildHeap(int startFromIndex);
Node* pop(void);
void printCurrentLevel(Node **root, int idx, int level);
void printLevelOrder(Node **root);
void assignCodesToChars(void);
void _assignCodesToChars(uint16_t bin, uint8_t binSize, Node *node);
void flatten(Node *node);
int serialize(uint8_t *inputString, int strSize, Node **encodingTable, uint8_t **compressedContent);
int getCharRepr(uint8_t c, Node **encodingTable, uint16_t *repr, uint8_t *reprSize);
int getNextBit(uint8_t *binary, int binSize);
int findChar(uint16_t value, uint16_t len, Node **encodingTable);


Node* Heap[MAX_HEAP_SIZE];
int size;
void registerChar(uint8_t ch, int weight, uint16_t bin, uint8_t binSize);
void* buildEncodingTable(void);
int buildEncodingSegment(encodingSegmentItem **encodingSegment, Node **encodingTable);
uint8_t* createImage(uint8_t* compressedContent, int sizeOfContentInBits, encodingSegmentItem* encodingSegment, int sizeOfencodingSegment, int sizeOfLengthSegment);
int restoreContentLength(uint8_t* archive);
Node** restoreEncodingTable(uint8_t* archive);
uint8_t* restoreContent(uint8_t* binContent, int contentLengthInBits, Node **encodingTable, int *restoredSize);
uint8_t* stripHeader(uint8_t* archive, int contentLengthInBits);
void compress(char *inFile, char *outputBinary);
void extract(char *inFile, char *outputBinary);
long getFileSize(char *filename);
void* gc[MAX_HEAP_SIZE + 32];
int gcCounter;
void freeAll(void);

#endif