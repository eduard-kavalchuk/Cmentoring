#include <stdio.h>
#include "set.h"

struct set_t {
    node_t root;
};

set_t Set(void) {
    set_t set = (set_t) malloc(sizeof(struct set_t));
    set->root = NULL;
    
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
}

void set_addAll(set_t set, int array[], int size)
{
    for (int i = 0; i < size; i++) {
        bst_insertNode(&set->root, array[i]);
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

int set_getSize(set_t set)
{
    return bst_getSize(set->root);
}

void set_toArray(set_t set, int *array, int size)
{
    bst_toArray(set->root, array, size);    
}

void set_delete(set_t set, int x)
{
    bst_delete(&set->root, x);
}
