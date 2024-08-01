#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* returns 1 if little endian and 0 if big endian */
bool endianness(void);

int main()
{
    if(endianness()) {
        printf("Little endian\n");
    }
    else {
        printf("Big endian\n");
    }
    
    return 0;
}

bool endianness(void)
{
    union {
        uint16_t s;
        unsigned char c[2];
    } u;
    
    u.s = 1;
    
    // On little endian system u.s is 0x0100 (so u.s[0] is 1), on big endian it is 0x0001 (so u.s[0] is 0)
    return u.c[0] == 1 ? true : false;
}
