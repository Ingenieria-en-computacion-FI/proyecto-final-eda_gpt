#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

/* Cola circular de enteros (PIDs)
   Uso: scheduler Round Robin */

typedef struct CircularQueue CircularQueue;

CircularQueue* cq_create(int capacity);   
void cq_enqueue(CircularQueue* cq, int value);
int cq_dequeue(CircularQueue* cq);
int cq_peek(CircularQueue* cq);
int cq_is_empty(CircularQueue* cq);
int cq_is_full(CircularQueue* cq);
int cq_size(CircularQueue* cq);
void cq_print(CircularQueue* cq);
void cq_destroy(CircularQueue* cq);

#endif