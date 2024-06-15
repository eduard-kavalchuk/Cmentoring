#include <stdio.h>
#include "bst.h"
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
    set->root = insertNode(set->root, value);
}

void set_addAll(set_t set, int array[], int size)
{
    for (int i = 0; i < size; i++)
        set->root = insertNode(set->root, array[i]);
}

void set_print(set_t set)
{
    printf("%p\n", (void *) set->root);
    inOrder(set->root);
    printf("\n");
}

void set_clear(set_t set)
{
    clear(&set->root);
}

bool set_isEmpty(set_t set)
{
    if (set->root == NULL)
        return true;
    return false;
}