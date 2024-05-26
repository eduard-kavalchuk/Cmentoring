#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

typedef struct stack_t * stack_t;

stack_t stack_new(void);
bool stack_empty(stack_t stk);
void stack_push(stack_t stk, void *x);
void *stack_pop(stack_t stk);
void stack_free(stack_t *stk);

#endif
