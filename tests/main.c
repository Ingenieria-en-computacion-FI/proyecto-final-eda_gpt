#include <stdio.h>

/* Declaraciones de todos los tests */
void test_stack_push_pop();
void test_stack_empty();
void test_stack_peek();

void test_queue_fifo();
void test_queue_empty();
void test_queue_size();

void test_round_robin_behavior();
void test_circular_resize();

void test_first_fit();
void test_best_fit();
void test_coalescence();
void test_fragmentation();

void test_fifo_scheduler();
void test_rr_scheduler();
void test_sjf_scheduler();

int main() {
    printf("========================================\n");
    printf("          SUITE DE TESTS                \n");
    printf("========================================\n");

    printf("\n[Stack]\n");
    test_stack_push_pop();
    test_stack_empty();
    test_stack_peek();

    printf("\n[Queue FIFO]\n");
    test_queue_fifo();
    test_queue_empty();
    test_queue_size();

    printf("\n[Cola Circular]\n");
    test_round_robin_behavior();
    test_circular_resize();

    printf("\n[Memory Manager]\n");
    test_first_fit();
    test_best_fit();
    test_coalescence();
    test_fragmentation();

    printf("\n[Schedulers]\n");
    test_fifo_scheduler();
    test_rr_scheduler();
    test_sjf_scheduler();

    printf("\n========================================\n");
    printf("   Todos los tests pasaron ✓          \n");
    printf("===========================================\n");
    return 0;
}