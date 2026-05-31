#include <stdio.h>
#include <stdlib.h>
#include "../include/stack.h"

struct Node {
    int value;
    struct Node* next;
};

struct Stack {
    struct Node* top;
};

Stack* stack_create() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->top = NULL;
    return stack;
}

void stack_push(Stack* stack, int value) {
}

int stack_pop(Stack* stack) {
    return -1;
}

int stack_peek(Stack* stack) {
    return -1;
}

int stack_is_empty(Stack* stack) {
    return 1;
}

void stack_destroy(Stack* stack) {
}
