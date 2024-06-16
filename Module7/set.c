#include <stdio.h>
#include "bst.h"
#include "set.h"

struct set_t {
    node_t root;
    int size;
};

set_t Set(void) {
    set_t set = (set_t) malloc(sizeof(struct set_t));
    set->root = NULL;
    set->size = 0;
    
    return set;
}

set_t set_fromArray(int array[], int size)
{
    set_t set = Set();
    for (int i = 0; i < size; i++) {
        set_add(set, array[i]);
    }
    return set;
}

void set_add(set_t set, int value)
{
    bst_insertNode(&set->root, value);
    set->size++;
}

void set_addAll(set_t set, int array[], int size)
{
    for (int i = 0; i < size; i++) {
        bst_insertNode(&set->root, array[i]);
        set->size++;
    }
}

void set_print(set_t set)
{
    bst_print(set->root);
    printf("\n");
}

void set_clear(set_t set)
{
    bst_clear(&set->root);
    set->size = 0;
}

bool set_isEmpty(set_t set)
{
    if (set->root == NULL)
        return true;
    return false;
}

int set_findMax(set_t set, int *storage)
{
    return bst_findMax(set->root, storage);
}

int set_findMin(set_t set, int *storage)
{
    return bst_findMin(set->root, storage);
}

int size(set_t set)
{
    return set->size;
}

void set_toArray(set_t set, int *array, int size)
{
    bst_toArray(set->root, array, size);    
}

void set_delete(set_t set, int x)
{
    bst_delete(&set->root, x);
}
