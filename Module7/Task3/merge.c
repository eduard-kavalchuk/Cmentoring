#include <stdio.h>

void mergeArrays(int array1[], int array2[], int size1, int size2, int array3[])
{
  int i = 0, j = 0, k = 0;
  
  while (i < size1 && j < size2) {
      if (array1[i] < array2[j])
          array3[k++] = array1[i++];
      else
          array3[k++] = array2[j++];
  }
  
  while (i < size1)
      array3[k++] = array1[i++];
 
  while (j < size2)
      array3[k++] = array2[j++];
}

void printArray(int array[], int size)
{
    for (int i = 0; i < size; i++)
        printf("%d ", array[i]);
    printf("\n");
}

int main(void)
{
    int array1[] = {1, 3, 5, 7, 9};
    int array2[] = {2, 4, 6, 8};
    int array3[(sizeof(array1) + sizeof(array2))/sizeof(int)];
    
    mergeArrays(array1, array2, sizeof(array1)/sizeof(int), sizeof(array2)/sizeof(int), array3);
    printArray(array3, sizeof(array3)/sizeof(int));
    
    return 0;
}
