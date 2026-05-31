#include <stdio.h>
#include <assert.h>
#include "stack.h"

void test_stack_push_pop() {
    Stack* stack = stack_create();
    stack_push(stack, 10);
    stack_push(stack, 20);
    stack_push(stack, 30);
    assert(stack_pop(stack) == 30);
    assert(stack_pop(stack) == 20);
    assert(stack_pop(stack) == 10);
    stack_destroy(stack);
    printf("  [OK] test_stack_push_pop\n");
}

void test_stack_empty() {
    Stack* stack = stack_create();
    assert(stack_is_empty(stack) == 1);
    stack_push(stack, 5);
    assert(stack_is_empty(stack) == 0);
    assert(stack_size(stack) == 1);
    stack_pop(stack);
    assert(stack_is_empty(stack) == 1);
    stack_destroy(stack);
    printf("  [OK] test_stack_empty\n");
}

void test_stack_peek() {
    Stack* stack = stack_create();
    stack_push(stack, 42);
    assert(stack_peek(stack) == 42);
    assert(stack_size(stack) == 1);  /* peek no extrae */
    stack_destroy(stack);
    printf("  [OK] test_stack_peek\n");
} 