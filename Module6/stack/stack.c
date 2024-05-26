#include <stdlib.h>
#include "stack.h"

#define DEFAULT_CAPACITY 2

struct stack_t {
	void **base;
	int size;
	int capacity;
};

stack_t stack_new(void)
{
	stack_t stack = (stack_t) malloc(sizeof(stack_t));
	stack->base = (void **) malloc(DEFAULT_CAPACITY * sizeof(void *));
	stack->capacity = DEFAULT_CAPACITY;
	stack->size = 0;
	
	return stack;
}

bool stack_empty(stack_t stack)
{
	return stack->size == 0;
}

void stack_push(stack_t stack, void *x)
{
	if (stack->capacity == stack->size) {
		stack->capacity *= 2;
		stack->base = (void **) realloc(stack->base, stack->capacity * sizeof(void *));
	}
	stack->base[stack->size++] = x;
}

void *stack_pop(stack_t stack)
{
	return stack->base[--stack->size];
}

void stack_free(stack_t *stack)
{
	free((*stack)->base);
	free(*stack);
}
