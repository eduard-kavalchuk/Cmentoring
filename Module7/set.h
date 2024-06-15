#ifndef SET_H
#define SET_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct set_t* set_t;

set_t Set(void);
set_t set_fromArray(int array[], int size);
void set_add(set_t set, int key);
void set_addAll(set_t set, int array[], int);
void set_print(set_t set);
void set_clear(set_t set);
bool set_isEmpty(set_t set);
int set_findMax(set_t set);
int set_findMin(set_t set);
int size(set_t set);
void set_toArray(set_t set, int *array, int size);

#endif