#include <stdio.h>
#include "bst.h"
#include "set.h"

#define SIZE 5

int main(void)
{
    int array[SIZE] = {6, 2, 78, 9, 12};
    int addon[3] = {111, 110, 109};
    
    set_t set = set_fromArray(array, SIZE);
    set_addAll(set, addon, 3);
    set_add(set, -333);
    set_print(set);
    /*
    printf("%d\n", set_isEmpty(set));
    set_clear(set);
    printf("%d\n", set_isEmpty(set));
    */
    set_clear(set);
    set_print(set);
    printf("%d\n", set_isEmpty(set));
}
