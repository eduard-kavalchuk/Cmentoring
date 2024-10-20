#ifndef API_H
#define API_H

typedef struct MinHeap MinHeap;
typedef struct Node Node;

struct MinHeap {
    void **arr;
    int size;
    int capacity;
    int (*compare)(void *e1, void *e2);
    void (*setWeight)(MinHeap *heap, void *e, int weight);
    int (*getWeight)(void *e);
};

int getParentIdx(int i);
int getLeftChildIdx(int i);
int getRightChildIdx(int i);
MinHeap* init_minheap(int capacity, int (*compare)(void *e1, void *e2),
                      void (*setWeight)(MinHeap *heap, void *e, int weight), int (*getWeight)(void *e));
MinHeap* insert_minheap(MinHeap* heap, void* element);
MinHeap* heapify(MinHeap* heap, int index);
void delete_minimum(MinHeap* heap);
void delete_element(MinHeap* heap, int index);
void free_minheap(MinHeap* heap);
void* peek(MinHeap* heap, int index);

#endif
