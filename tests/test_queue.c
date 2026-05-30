#include <stdio.h>
#include <assert.h>
#include "queue.h"

void test_queue_fifo() {
    Queue* queue = queue_create();
    enqueue(queue, 1);
    enqueue(queue, 2);
    enqueue(queue, 3);
    assert(dequeue(queue) == 1);
    assert(dequeue(queue) == 2);
    assert(dequeue(queue) == 3);
    queue_destroy(queue);
    printf("  [OK] test_queue_fifo\n");
}

void test_queue_empty() {
    Queue* queue = queue_create();
    assert(queue_is_empty(queue) == 1);
    enqueue(queue, 99);
    assert(queue_is_empty(queue) == 0);
    dequeue(queue);
    assert(queue_is_empty(queue) == 1);
    queue_destroy(queue);
    printf("  [OK] test_queue_empty\n");
}

void test_queue_size() {
    Queue* queue = queue_create();
    enqueue(queue, 1);
    enqueue(queue, 2);
    assert(queue_size(queue) == 2);
    dequeue(queue);
    assert(queue_size(queue) == 1);
    queue_destroy(queue);
    printf("  [OK] test_queue_size\n");
} 