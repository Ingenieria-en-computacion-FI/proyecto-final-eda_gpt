/*
 * sjf.c — Scheduler Shortest Job First (SJF)
 *
 * Ordena los procesos por burst_time ascendente (Greedy).
 * El proceso más corto se ejecuta primero.
 * Es óptimo en tiempo de espera promedio (no apropiativo).
 *
 * Complejidad temporal:
 *   Ordenar: O(n log n)  — merge sort
 *   Ejecutar: O(n)
 *   Total: O(n log n)
 *
 * Recurrencia Merge Sort:
 *   T(n) = 2T(n/2) + O(n)
 *   Por teorema maestro (caso 2): T(n) = O(n log n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"
#include "algorithms.h"

/* ── Crear scheduler SJF ── */
Scheduler* scheduler_create_sjf() {
    Scheduler* s = (Scheduler*)malloc(sizeof(Scheduler));
    if (!s) {
        fprintf(stderr, "scheduler_create_sjf: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    s->type          = SCHED_SJF;
    s->count         = 0;
    s->quantum       = 0;
    s->current_time  = 0;
    s->completed     = 0;
    s->fifo_queue    = NULL;
    s->rr_queue      = NULL;
    s->finished_list = ll_create();
    s->history       = stack_create();
    memset(s->processes, 0, sizeof(s->processes));
    return s;
}

/* ── Ejecutar simulación SJF ── */
void scheduler_run_sjf(Scheduler* s) {
    /* Ordenar por burst_time (Greedy/Merge Sort) */
    greedy_sjf_sort(s->processes, s->count);

    printf("\n=== Simulación SJF (Shortest Job First) ===\n");
    printf("%-6s %-10s %-8s %-8s\n", "PID", "Burst", "Inicio", "Fin");

    for (int i = 0; i < s->count; i++) {
        Process* p = &s->processes[i];
        p->state          = RUNNING;
        int start_time    = s->current_time;
        s->current_time  += p->burst_time;
        p->remaining_time = 0;
        p->state          = FINISHED;

        stack_push(s->history, p->pid);
        ll_insert_sorted(s->finished_list, p->pid, p->burst_time);
        s->completed++;

        printf("%-6d %-10d %-8d %-8d\n",
               p->pid, p->burst_time, start_time, s->current_time);
    }

    printf("\nTiempo total: %d unidades\n", s->current_time);
} 