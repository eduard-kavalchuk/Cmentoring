// Implement a program to extract and print individual bytes from an unsigned int using the following union

#include <stdio.h>

union U {
    unsigned int a;
    unsigned char s[4];
};

int main(void)
{
    union U u = {0xAABBCCDD};
    
    /*
        Bytes in s[] array are located in order of increasing addresses (by definition of array).
        That means addr(s[1]) == addr(s[0]) + 1, addr(s[2]) == addr(s[1]) + 1, etc.
        If the order is s[0] == 0xDD, s[1] == 0xCC, s[2] == 0xBB, s[3] == 0xAA then the system is little endian.
        Otherwise, it is big endian.
        Intel is little endian.
    */
    printf("0x%x 0x%x 0x%x 0x%x\n", u.s[0], u.s[1], u.s[2], u.s[3]);
    
    return 0;
}
