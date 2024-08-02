#include <stdio.h>

int swap_endianness(int num);

int main(void)
{
    int n = 0xAABBCCDD;
    
    printf("input = 0x%X\n", n);
    printf("output = 0x%X\n", swap_endianness(n));
    
    return 0;
}

int swap_endianness(int num)
{
    union s {
        int number;
        char c[sizeof(int)];
    } in;
    
    in.number = num;
    for(int i = 0; i < sizeof(num) / 2; i++) {
        char k = in.c[i];
        in.c[i] = in.c[sizeof(num) - i - 1];
        in.c[sizeof(num) - i - 1] = k;
    }
    return in.number;
}
