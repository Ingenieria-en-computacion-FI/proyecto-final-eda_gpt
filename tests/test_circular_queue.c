#include <stdio.h>
#include <assert.h>
#include "circular_queue.h"

void test_round_robin_behavior() {
    CircularQueue* cq = cq_create(4);
    cq_enqueue(cq, 1);
    cq_enqueue(cq, 2);
    cq_enqueue(cq, 3);
    assert(cq_dequeue(cq) == 1);
    cq_enqueue(cq, 4);              /* Reinsertar al final */
    assert(cq_dequeue(cq) == 2);
    assert(cq_dequeue(cq) == 3);
    assert(cq_dequeue(cq) == 4);
    assert(cq_is_empty(cq) == 1);
    cq_destroy(cq);
    printf("  [OK] test_round_robin_behavior\n");
}

void test_circular_resize() {
    CircularQueue* cq = cq_create(2);
    cq_enqueue(cq, 10);
    cq_enqueue(cq, 20);
    cq_enqueue(cq, 30);  /* Debe redimensionar */
    assert(cq_size(cq) == 3);
    assert(cq_dequeue(cq) == 10);
    cq_destroy(cq);
    printf("  [OK] test_circular_resize\n");
}