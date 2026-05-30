#ifndef QUEUE_H
#define QUEUE_H

/* Cola FIFO de enteros (PIDs)
   Uso: scheduler FIFO */

typedef struct Queue Queue;

Queue* queue_create();
void   enqueue(Queue* queue, int value);
int    dequeue(Queue* queue);
int    queue_peek(Queue* queue);
int    queue_is_empty(Queue* queue);
int    queue_size(Queue* queue);
void   queue_print(Queue* queue);
void   queue_destroy(Queue* queue);

#endif
