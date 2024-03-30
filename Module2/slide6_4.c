#include <stdio.h>

/*
        .file   "slide6_4.c"
        .text
        .globl  p
        .section        .rodata
.LC0:
        .string "value %zu\n"
        .section        .data.rel.ro.local,"aw"
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

char *const p = "value %zu\n";

int main(void)
{
	printf(p, sizeof(5));
	
	return 0;
}

