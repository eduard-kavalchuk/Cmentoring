#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Node Node;

struct Node {
    int weight;
    int character;
    char *repr;
    Node *nextLevelNodes[2];
};

static char *inputStr = "preved medved";
char inputArray[255];
Node* heap[255];
int heapStart = 0;
int heapEnd = 0;
int size = 0;



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

void printCurrentLevel(int idx, int level) {
    if (idx >= size)
        return;
    if (level == 1) {
        if (heap[idx]->character == ' ')
            printf("('', %d)  ", heap[idx]->weight);
        else
            printf("(%c, %d)  ", heap[idx]->character, heap[idx]->weight);
    }
    else if (level > 1) {
        printCurrentLevel(getLeftChildIdx(idx), level - 1);
        printCurrentLevel(getRightChildIdx(idx), level - 1);
    }
}

void printLevelOrder() {
    int h = height();
    
    for (int i = 1; i <= h; i++) {
        printCurrentLevel(0, i);
        printf("\n");
    }
}


void insert(Node** heap, Node* element) {
    heap[size++] = element;

    int curr = size - 1;

    while (curr > 0 &&  heap[getParentIdx(curr)]->weight > heap[curr]->weight) {
        Node *temp = heap[getParentIdx(curr)];
        heap[getParentIdx(curr)] = heap[curr];
        heap[curr] = temp;
        curr = getParentIdx(curr);
    }
    
    return; 
}



int main()
{
    for (int i = 0; i < strlen(inputStr); i++) {
        inputArray[(int) inputStr[i]] += 1;
    }
    
    for (int i = 0; i < 254; i++) {
        if (!inputArray[i]) continue;
        
        Node *node = (Node *) calloc(1, sizeof(Node));
        node->weight = inputArray[i];
        node->character = i;
        insert(heap, node);
    }
    
    printLevelOrder();
    
    return 0;
}