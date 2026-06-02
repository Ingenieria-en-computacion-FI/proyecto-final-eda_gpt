#ifndef STACK_H
#define STACK_H

/* Pila genérica de enteros (PIDs)
   Uso: historial de ejecución, backtracking */

typedef struct Stack Stack;
   
Stack* stack_create();
void stack_push(Stack* stack, int value);
int stack_pop(Stack* stack);
int stack_peek(Stack* stack);
int stack_is_empty(Stack* stack);
int stack_size(Stack* stack);
void stack_print(Stack* stack);
void stack_destroy(Stack* stack);

#endif 