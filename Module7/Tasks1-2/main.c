#include <stdio.h>
#include "bst.h"
#include "set.h"

#define SIZE 10

int count_duplicates(int array[], int size)
{
    set_t set = set_fromArray(array, size);
    
    return size - set_getSize(set);
}

void minmax(int array[], int size, int *min, int *max)
{
    set_t set = set_fromArray(array, size);
    set_findMax(set, max);
    set_findMin(set, min);
}

int main(void)
{
    int array[SIZE] = {1, 3, 2, 4, 3, 6, 1, 3, 2, -5};
    int min, max;
    
    printf("number of duplicates: %d\n", count_duplicates(array, SIZE));
    
    minmax(array, SIZE, &min, &max);
    printf("min: %d\n", min);
    printf("max: %d\n", max);
    
    return 0;
}
