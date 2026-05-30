/*
 * circular_queue.c — Cola circular basada en arreglo
 *
 * Complejidad temporal:
 *   enqueue : O(1) amortizado  (redimensiona al doble si está llena)
 *   dequeue : O(1)
 *   peek    : O(1)
 *
 * Complejidad espacial: O(n)
 *
 * Uso en el sistema: scheduler Round Robin.
 * La cola circular permite reinsertar procesos al final
 * de forma eficiente, sin mover elementos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circular_queue.h"

struct CircularQueue {
    int* data;
    int  head;
    int  tail;
    int  size;
    int  capacity;
};

/* ── Crear cola circular con capacidad inicial ── */
CircularQueue* cq_create(int capacity) {
    if (capacity <= 0) capacity = 8;
    CircularQueue* cq = (CircularQueue*)malloc(sizeof(CircularQueue));
    if (!cq) {
        fprintf(stderr, "cq_create: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    cq->data     = (int*)malloc(sizeof(int) * capacity);
    cq->head     = 0;
    cq->tail     = 0;
    cq->size     = 0;
    cq->capacity = capacity;
    return cq;
}

/* ── Redimensionar al doble cuando está llena ── */
static void cq_resize(CircularQueue* cq) {
    int new_cap = cq->capacity * 2;
    int* new_data = (int*)malloc(sizeof(int) * new_cap);
    /* Copiar en orden lógico */
    for (int i = 0; i < cq->size; i++)
        new_data[i] = cq->data[(cq->head + i) % cq->capacity];
    free(cq->data);
    cq->data     = new_data;
    cq->head     = 0;
    cq->tail     = cq->size;
    cq->capacity = new_cap;
}

/* ── Insertar al final: O(1) amortizado ── */
void cq_enqueue(CircularQueue* cq, int value) {
    if (cq_is_full(cq))
        cq_resize(cq);
    cq->data[cq->tail] = value;
    cq->tail = (cq->tail + 1) % cq->capacity;
    cq->size++;
}

/* ── Extraer del frente: O(1) ── */
int cq_dequeue(CircularQueue* cq) {
    if (cq_is_empty(cq)) {
        fprintf(stderr, "cq_dequeue: cola vacía\n");
        return -1;
    }
    int value = cq->data[cq->head];
    cq->head  = (cq->head + 1) % cq->capacity;
    cq->size--;
    return value;
}

/* ── Ver frente sin extraer: O(1) ── */
int cq_peek(CircularQueue* cq) {
    if (cq_is_empty(cq)) return -1;
    return cq->data[cq->head];
}

int cq_is_empty(CircularQueue* cq) { return cq->size == 0; }
int cq_is_full(CircularQueue* cq)  { return cq->size == cq->capacity; }
int cq_size(CircularQueue* cq)     { return cq->size; }

/* ── Imprimir cola circular ── */
void cq_print(CircularQueue* cq) {
    printf("CircularQueue [frente -> final, size=%d]: ", cq->size);
    for (int i = 0; i < cq->size; i++)
        printf("%d ", cq->data[(cq->head + i) % cq->capacity]);
    printf("\n");
}

/* ── Destruir ── */
void cq_destroy(CircularQueue* cq) {
    free(cq->data);
    free(cq);
}