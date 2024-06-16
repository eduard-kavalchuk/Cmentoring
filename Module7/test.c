#include <stdio.h>
#include "bst.h"
#include "set.h"

#define SIZE 5

int main(void)
{
    int array[SIZE] = {1, 3, 2, 4, 6};
    
    /*
    int addon[3] = {111, 110, 109};
    
    set_t set = set_fromArray(array, SIZE);
    printf("size = %d\n", size(set));
    set_addAll(set, addon, 3);
    printf("size = %d\n", size(set));
    set_add(set, -333);
    printf("size = %d\n", size(set));
    set_print(set);
    printf("Min = %d\n", set_findMin(set));
    printf("Max = %d\n", set_findMax(set));
    */
    
    /*
    set_t set = set_fromArray(array, SIZE);
    set_toArray(set, array, SIZE);
    for (int i = 0; i < SIZE; i++)
        printf("%d ", array[i]);
    printf("\n");
    set_delete(set, 12);
    set_print(set);
    int x;
    set_findMax(set, &x);
    printf("%d\n", x);
    set_findMin(set, &x);
    printf("%d\n", x);
    */
    
    set_t set = set_fromArray(array, SIZE);
    set_print(set);
    set_delete(set, 4);
    set_print(set);
    
    return 0;
}
