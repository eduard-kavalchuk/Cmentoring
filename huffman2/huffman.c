#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Node Node;

struct Node {
    int weight;
    int character;
    char *repr;
    int isLeaf;
    Node *nextLevelNodes[2];
};


static char *inputStr = "preved medved";
char inputArray[255];
Node* heap[255];
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

void printCurrentLevel(Node **root, int idx, int level) {
    if (idx >= size)
        return;
    if (level == 1) {
        if (root[idx]->character == ' ')
            printf("('', %d)  ", root[idx]->weight);
        else if(root[idx]->character > 127)
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
    if (node->character < 127)
        printf("(%c, %d) ", node->character, node->weight);
    else
        printf("(%d, %d) ", node->character, node->weight);
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

void heapify(int index) {
    // Rearranges the heap as to maintain
    // the min-heap property
    if (size <= 1)
        return;
    
    int left = getLeftChildIdx(index); 
    int right = getRightChildIdx(index); 
    

    // Variable to get the smallest element of the subtree
    // of an element an index
    int smallest = index; 
    
    // If the left child is smaller than this element, it is
    // the smallest
    if (left < size && heap[left]->weight < heap[index]->weight )
        smallest = left; 
    
    // Similarly for the right, but we are updating the smallest element
    // so that it will definitely give the least element of the subtree
    if (right < size && heap[right]->weight  < heap[smallest]->weight ) 
        smallest = right; 

    // Now if the current element is not the smallest,
    // swap with the current element. The min heap property
    // is now satisfied for this subtree. We now need to
    // recursively keep doing this until we reach the root node,
    // the point at which there will be no change!
    if (smallest != index) 
    { 
        //printf("Before swap:\n");
        //printLevelOrder(heap);
        //printf("index = %d, left = %d, right = %d, smallest = %d\n", index, left, right, smallest);
        Node *temp = heap[index];
        heap[index] = heap[smallest];
        heap[smallest] = temp;
        //printf("After swap:\n");
        //printLevelOrder(heap);
        //printf("================================\n");
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


void repr(char *str, Node *node) {
    if (node == NULL) return;
    
    if (node->isLeaf) {
        node->repr = (char *) malloc(strlen(str));
        strcpy(node->repr, str);
    }
    
    char *s1 = (char *) malloc(strlen(str) + 1);
    strcpy(s1, str);
    strcat(s1, "0");
    repr(s1, node->nextLevelNodes[0]);
    
    char *s2 = (char *) malloc(strlen(str) + 1);
    strcpy(s2, str);
    strcat(s2, "1");
    repr(s2, node->nextLevelNodes[1]);
    
}


void join()
{
    Node *node1, *node2;
    node1 = pop();
    node2 = pop();
    Node *node = (Node *) calloc(1, sizeof(Node));
    node->weight = node1->weight + node2->weight;
    node->character = 128;
    node->nextLevelNodes[0] = node1;
    node->nextLevelNodes[1] = node2;
    node->isLeaf = 0;
    insert(heap, node);
}

void flatten(Node *node)
{
    if (node == NULL) return;
    
    if (node->isLeaf) {      
        printf("\n(%c, %s)", node->character, node->repr);
        heap[size++] = node;
    }
    
    flatten(node->nextLevelNodes[0]);
    flatten(node->nextLevelNodes[1]);
}

int find_code(char c)
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
        return -1;
    return position;
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

char find_key(char *key)
{
    int position = 0, left = 0, right = size - 1;
    while (left <= right) {
        int middle = left + (right - left) / 2;
        int cmp = strcmp(heap[middle]->repr, key);
        if (cmp >= 0) {
            right = middle - 1;
            position = middle;
        } else {
            left = middle + 1;
        }
    }
    if (strcmp(heap[position]->repr, key) != 0)
        return -1;
    return heap[position]->character;
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
        node->isLeaf = 1;
        insert(heap, node);
    }
    
    printf("Initial heap:\n");
    printLevelOrder(heap);
    
    while(size != 1) {
        join();
        printf("===================================\n");
        printLevelOrder(heap);
    }
    
    repr("", heap[0]);
    
    flatten(pop());
    
    printf("\n");
    for (int i = 0; i < size; i++) {
        printf("(%c, %s) ", heap[i]->character, heap[i]->repr);
    }
    
    qsort(heap, size, sizeof(Node*), compare_by_char);
    
    printf("\n");
    for (int i = 0; i < size; i++) {
        printf("(%c, %s) ", heap[i]->character, heap[i]->repr);
    }
    
    
    
    // Convert input string to binary code
    printf("\n");
    char *s3 = (char *) malloc(300);
    for (int i = 0 ; i < strlen(inputStr); i++) {
        int v = find_code(inputStr[i]);
        //printf("(%c, %d) ", inputStr[i], v);
        strcat(s3, heap[v]->repr);
    }
    
    printf("\nBinary output:\n%s\n", s3);
    
    
    
    qsort(heap, size, sizeof(Node*), compare_by_binary);
    
    printf("\n");
    for (int i = 0; i < size; i++) {
        printf("(%c, %s) -> ", heap[i]->character, heap[i]->repr);
    }
    
    
    
    
    char *decoded_string = (char *) calloc(1, 100);
    char *tmp = (char *) calloc(1, 100);
    int total_decoded = 0;
    int start = 0, end = 1;
    char c;
    
    while(end <= strlen(s3)) {
        strncpy(tmp, s3 + start, end - start);
        if ((c = find_key(tmp)) != -1) {
            decoded_string[total_decoded++] = c;
            start = end;
            memset(tmp, '\0', strlen(tmp));
        }
        ++end;
        
    }
    
    printf("\nDecoded string = %s\n", decoded_string);
    
    
    
    
    return 0;
}
