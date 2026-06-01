/*
 * stack.c — Pila implementada con lista enlazada
 *
 * Complejidad temporal:
 *   push  : O(1)
 *   pop   : O(1)
 *   peek  : O(1)
 *
 * Complejidad espacial: O(n)
 *
 * Uso en el sistema: historial de ejecución de procesos (backtracking).
 */

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/* ── Nodo interno ── */
typedef struct Node {
    int          value;
    struct Node* next;
} Node;

/* ── Estructura opaca ── */
struct Stack {
    Node* top;
    int   size;
};

/* ── Crear pila ── */
Stack* stack_create() {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    if (!s) {
        fprintf(stderr, "stack_create: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    s->top  = NULL;
    s->size = 0;
    return s;
}

/* ── Insertar en tope: O(1) ── */
void stack_push(Stack* stack, int value) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) {
        fprintf(stderr, "stack_push: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    node->value = value;
    node->next  = stack->top;
    stack->top  = node;
    stack->size++;
}

/* ── Extraer del tope: O(1) ── */
int stack_pop(Stack* stack) {
    if (stack_is_empty(stack)) {
        fprintf(stderr, "stack_pop: stack vacío\n");
        return -1;
    }
    Node* tmp   = stack->top;
    int   value = tmp->value;
    stack->top  = tmp->next;
    free(tmp);
    stack->size--;
    return value;
}

/* ── Ver tope sin extraer: O(1) ── */
int stack_peek(Stack* stack) {
    if (stack_is_empty(stack)) return -1;
    return stack->top->value;
}

/* ── Verificar si está vacía: O(1) ── */
int stack_is_empty(Stack* stack) {
    return stack->top == NULL;
}

/* ── Tamaño actual: O(1) ── */
int stack_size(Stack* stack) {
    return stack->size;
}

/* ── Imprimir pila ── */
void stack_print(Stack* stack) {
    printf("Stack [tope -> fondo]: ");
    Node* cur = stack->top;
    while (cur) {
        printf("%d ", cur->value);
        cur = cur->next;
    }
    printf("\n");
}

/* ── Destruir pila: O(n) ── */
void stack_destroy(Stack* stack) {
    while (!stack_is_empty(stack))
        stack_pop(stack);
    free(stack);
}
