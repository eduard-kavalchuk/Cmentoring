// Implement a function to compare the structure variables.

/*
    Only structures of known types can be compared. Comparison happens filed-by-field.
    1. Integers are compared by direct comparison.
    2. Strings are compared by their content using *strcmp*.
    3. Embedded structures are compared field-by-field.
*/

#include <stdio.h>
#include <string.h>

struct T {
    int integer;
    char *string;
    struct B *b;
};

struct B {
    float f;
};

int cmp(const struct T *a, const struct T *b);

int main(void)
{
    float f = 1.23F;
    struct B b1 = {f};
    struct B b2 = {f};
    
    struct T t1 = {1, "Name1", &b1};
    struct T t2 = {1, "Name1", &b2};
    
    printf("%d\n", cmp(&t1, &t2));
    
    return 0;
}

int cmp(const struct T *a, const struct T *b)
{
    if (a->integer != b->integer)
        return 0;
    
    if (strcmp(a->string, b->string) != 0)
        return 0;
    
    if (a->b->f != b->b->f)
        return 0;
    
    return 1;
}
