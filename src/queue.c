/*
 * queue.c — Cola FIFO implementada con lista enlazada
 *
 * Complejidad temporal:
 *   enqueue : O(1)  — inserta al final
 *   dequeue : O(1)  — extrae del frente
 *   peek    : O(1)
 *
 * Complejidad espacial: O(n)
 *
 * Uso en el sistema: scheduler FIFO.
 */

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/* ── Nodo interno ── */
typedef struct QNode {
    int           value;
    struct QNode* next;
} QNode;

/* ── Estructura opaca ── */
struct Queue {
    QNode* front;
    QNode* rear;
    int    size;
};

/* ── Crear cola ── */
Queue* queue_create() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (!q) {
        fprintf(stderr, "queue_create: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    q->front = NULL;
    q->rear  = NULL;
    q->size  = 0;
    return q;
}

/* ── Insertar al final: O(1) ── */
void enqueue(Queue* queue, int value) {
    QNode* node = (QNode*)malloc(sizeof(QNode));
    if (!node) {
        fprintf(stderr, "enqueue: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    node->value = value;
    node->next  = NULL;

    if (queue->rear == NULL) {
        queue->front = node;
        queue->rear  = node;
    } else {
        queue->rear->next = node;
        queue->rear       = node;
    }
    queue->size++;
}

/* ── Extraer del frente: O(1) ── */
int dequeue(Queue* queue) {
    if (queue_is_empty(queue)) {
        fprintf(stderr, "dequeue: cola vacía\n");
        return -1;
    }
    QNode* tmp   = queue->front;
    int    value = tmp->value;
    queue->front = tmp->next;
    if (queue->front == NULL)
        queue->rear = NULL;
    free(tmp);
    queue->size--;
    return value;
}

/* ── Ver frente sin extraer: O(1) ── */
int queue_peek(Queue* queue) {
    if (queue_is_empty(queue)) return -1;
    return queue->front->value;
}

/* ── Verificar si está vacía: O(1) ── */
int queue_is_empty(Queue* queue) {
    return queue->front == NULL;
}

/* ── Tamaño actual: O(1) ── */
int queue_size(Queue* queue) {
    return queue->size;
}

/* ── Imprimir cola ── */
void queue_print(Queue* queue) {
    printf("Queue [frente -> final]: ");
    QNode* cur = queue->front;
    while (cur) {
        printf("%d ", cur->value);
        cur = cur->next;
    }
    printf("\n");
}

/* ── Destruir cola: O(n) ── */
void queue_destroy(Queue* queue) {
    while (!queue_is_empty(queue))
        dequeue(queue);
    free(queue);
}
