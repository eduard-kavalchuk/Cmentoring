#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define M 0
#define N 100

// https://c-faq.com/lib/randrange.html
#define get_random_int(M, N) (M + rand() / (RAND_MAX / ((N) - (M) + 1) + 1))

void sort(int arr[], int len, int (*compare)(int, int));
int comparator(int i1, int i2);
void printArray(int arr[], int size, char *msg);
int *generate_random_array(int n);

int main(void)
{
    int size = 5;
	int *a = generate_random_array(size);
    printArray(a, size, "Before sorting");
    sort(a, size, comparator);
	printArray(a, size, "After sorting");
}

void printArray(int arr[], int size, char *msg)
{ 
    printf("%s: ", msg);
	for (int i = 0; i < size; i++) 
        printf("%d ", arr[i]); 
    printf("\n");
} 

void sort(int arr[], int n, int (*compare)(int, int))
{ 
    int i, j;
    
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (compare(arr[j], arr[j + 1]) > 0) {
                int k = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = k;
            }
        }
    }
} 

int *generate_random_array(int n)
{
	int *pi = (int *) malloc(n * sizeof(int));
	
	srand(time(NULL));
	for (int i = 0; i < n; i++)
		pi[i] = get_random_int(M, N);
	
	return pi;
}

int comparator(int i1, int i2)
{
    if (i1 > i2)
        return 1;
    else if (i1 < i2)
        return -1;
    else return 0;
}
