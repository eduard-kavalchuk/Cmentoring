#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MinHeap MinHeap;

struct MinHeap {
    void **arr;
    int size;
    int capacity;
    int (*compare)(void *e1, void *e2);
    void (*setWeight)(void *e, int weight);
    int (*getWeight)(void *e);
};

int parent(int i);
int left_child(int i);
int right_child(int i);
int get_min(MinHeap *heap, int (*min_finder)(MinHeap *heap));
MinHeap* init_minheap(
    int capacity, 
    int (*compare)(void *e1, void *e2), 
    void (*setWeight)(void *e, int weight),
    int (*getWeight)(void *e)
);
MinHeap* insert_minheap(MinHeap* heap, void *element);
MinHeap* heapify(MinHeap* heap, int index);
void* delete_minimum(MinHeap* heap);
void* delete_element(MinHeap* heap, int index);
void free_minheap(MinHeap* heap);



