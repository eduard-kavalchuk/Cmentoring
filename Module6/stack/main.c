#include <stdio.h>
#include "stack.h"

int main(void)
{
	int i = 123, *pi;
	float f = 1.23, *pf;
	struct item {
		int id;
		char *name;
	} item, *itemPtr;
	
	stack_t stack = stack_new();
	printf("Is stack empty: %d\n", stack_empty(stack));
	
	stack_push(stack, (void *) &i);
	stack_push(stack, (void *) &f);
	
	item.id = 1;
	item.name = "Name of item";
	stack_push(stack, (void *) &item);
	
	itemPtr = (struct item *) stack_pop(stack);
	printf("Item id: %d\n", itemPtr->id);
	printf("Item name: %s\n", itemPtr->name);
	
	pf = (float *) stack_pop(stack);
	printf("Float: %g\n", *pf);
	
	pi = (int *) stack_pop(stack);
	printf("Integer: %d\n", *pi);
	
	stack_free(&stack);
	
	return 0;
}
