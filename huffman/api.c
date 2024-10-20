#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"


int getParentIdx(int i) {
    return (i - 1) / 2;
}

int getLeftChildIdx(int i) {
    return (2 * i + 1);
}

int getRightChildIdx(int i) {
    return (2 * i + 2);
}

MinHeap* init_minheap(int capacity, int (*compare)(void *e1, void *e2),
                      void (*setWeight)(MinHeap *heap, void *e, int weight), int (*getWeight)(void *e))
{
    MinHeap* minheap = (MinHeap*) calloc (1, sizeof(MinHeap));
    minheap->arr = (void **) calloc (capacity, sizeof(void *));
    minheap->capacity = capacity;
    minheap->size = 0;
    minheap->compare = compare;
    minheap->setWeight = setWeight;
    minheap->getWeight = getWeight;
    
    return minheap;
}

MinHeap* insert_minheap(MinHeap* heap, void* element) {
    // Inserts an element to the min heap
    // We first add it to the bottom (last level)
    // of the tree, and keep swapping with it's getParentIdx
    // if it is lesser than it. We keep doing that until
    // we reach the root node. So, we will have inserted the
    // element in it's proper position to preserve the min heap property
    if (heap->size == heap->capacity) {
        fprintf(stderr, "\nCannot insert. Heap is already full!\n");
        return heap;
    }
    // We can add it. Increase the size and add it to the end
    heap->size++;
    heap->arr[heap->size - 1] = element;

    // Keep swapping until we reach the root
    int curr = heap->size - 1;
    // As long as you aren't in the root node, and while the 
    // getParentIdx of the last element is greater than it
    while (curr > 0 && heap->compare(heap->arr[getParentIdx(curr)], heap->arr[curr]) > 0) {
        // Swap
        void *temp = heap->arr[getParentIdx(curr)];
        heap->arr[getParentIdx(curr)] = heap->arr[curr];
        heap->arr[curr] = temp;
        // Update the current index of element
        curr = getParentIdx(curr);
    }
    return heap; 
}

MinHeap* heapify(MinHeap* heap, int index) {
    // Rearranges the heap as to maintain
    // the min-heap property
    if (heap->size <= 1)
        return heap;
    
    int left = getLeftChildIdx(index); 
    int right = getRightChildIdx(index); 

    // Variable to get the smallest element of the subtree
    // of an element an index
    int smallest = index; 
    
    // If the left child is smaller than this element, it is
    // the smallest
    if (left < heap->size && heap->compare(heap->arr[left], heap->arr[index]) < 0)
        smallest = left; 
    
    // Similarly for the right, but we are updating the smallest element
    // so that it will definitely give the least element of the subtree
    if (right < heap->size && heap->compare(heap->arr[right], heap->arr[smallest]) < 0) 
        smallest = right; 

    // Now if the current element is not the smallest,
    // swap with the current element. The min heap property
    // is now satisfied for this subtree. We now need to
    // recursively keep doing this until we reach the root node,
    // the point at which there will be no change!
    if (smallest != index) 
    { 
        void *temp = heap->arr[index];
        heap->arr[index] = heap->arr[smallest];
        heap->arr[smallest] = temp;
        heap = heapify(heap, smallest); 
    }

    return heap;
}

void delete_minimum(MinHeap* heap) {
    // Deletes the minimum element, at the root
    if (!heap || heap->size == 0)
        return;
    
    // Update root value with the last element
    heap->arr[0] = heap->arr[heap->size - 1];;

    // Now remove the last element, by decreasing the size
    heap->size--;

    // We need to call heapify(), to maintain the min-heap property
    heapify(heap, 0);

}

void* peek(MinHeap* heap, int index)
{
    return heap->arr[index];
}

void delete_element(MinHeap* heap, int index) {
    // Deletes an element, indexed by index
    // Ensure that it's lesser than the current root
    
    heap->setWeight(heap, heap->arr[index], heap->getWeight(heap->arr[0]) - 1);
    
    // Now keep swapping, until we update the tree
    int curr = index;
    while (curr > 0 && heap->compare(heap->arr[getParentIdx(curr)], heap->arr[curr]) > 0) {
        void *temp = heap->arr[getParentIdx(curr)];
        heap->arr[getParentIdx(curr)] = heap->arr[curr];
        heap->arr[curr] = temp;
        curr = getParentIdx(curr);
    }

    // Now simply delete the minimum element
    delete_minimum(heap);
    
    return;
}



