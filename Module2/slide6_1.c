#include <stdio.h>

/*
        .file   "slide6_1.c"
        .text
        .section        .rodata
.LC0:
        .string "value %zu\n"
        .text
        .globl  main
        .type   main, @function
main:

*/

int main(void)
{
	char *p = "value %zu\n";
	
	printf(p, sizeof(5));
	
	return 0;
}

