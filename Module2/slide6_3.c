#include <stdio.h>

/*
        .file   "slide6_3.c"
        .text
        .globl  p
        .section        .rodata
.LC0:
        .string "value %zu\n"
        .section        .data.rel.local,"aw"
        .align 8
        .type   p, @object
        .size   p, 8
p:
        .quad   .LC0
        .text
        .globl  main
        .type   main, @function
main:

*/

char *p = "value %zu\n";

int main(void)
{
	printf(p, sizeof(5));
	
	return 0;
}

