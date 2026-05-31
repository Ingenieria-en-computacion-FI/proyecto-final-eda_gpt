#include <stdio.h>
#include <assert.h>
#include "scheduler.h"

void test_fifo_scheduler() {
    Scheduler* s = scheduler_create_fifo();
    scheduler_add_process(s, 1, 5);
    scheduler_add_process(s, 2, 3);
    scheduler_add_process(s, 3, 7);
    int first = scheduler_next(s);
    assert(first == 1);  /* FIFO: primero que entró */
    int second = scheduler_next(s);
    assert(second == 2);
    scheduler_destroy(s);
    printf("  [OK] test_fifo_scheduler\n");
}

void test_rr_scheduler() {
    Scheduler* s = scheduler_create_rr(2);
    scheduler_add_process(s, 1, 5);
    scheduler_add_process(s, 2, 3);
    scheduler_add_process(s, 3, 4);
    scheduler_run_rr(s);
    assert(s->completed == 3);
    scheduler_destroy(s);
    printf("  [OK] test_rr_scheduler\n");
}

void test_sjf_scheduler() {
    Scheduler* s = scheduler_create_sjf();
    scheduler_add_process(s, 1, 8);
    scheduler_add_process(s, 2, 2);
    scheduler_add_process(s, 3, 5);
    scheduler_run_sjf(s);
    /* SJF: debe terminar en orden P2(2) → P3(5) → P1(8) */
    assert(s->completed == 3);
    assert(s->current_time == 15);
    scheduler_destroy(s);
    printf("  [OK] test_sjf_scheduler\n");
} 